# frozen_string_literal: true

# TOGGLE API usage for translations; set to false to skip API calls and return source text, 500_000 characters per month
TOGGLE_API = false

# LANGUAGE PROXY CONFIGURATION
# change this array for your supported languages, english is implicit, default, and can't be removed, but please don't
# remove it either!
LANGUAGES = %w[en de ja]

# Activate and configure extensions
# https://middlemanapp.com/advanced/configuration/#configuring-extensions

activate :autoprefixer do |prefix|
  prefix.browsers = 'last 2 versions'
end

# Layouts
# https://middlemanapp.com/basics/layouts/

# Per-page layout changes
page '/*.xml', layout: false
page '/*.json', layout: false
page '/*.txt', layout: false

# With alternative layout
# page '/path/to/file.html', layout: 'other_layout'

# Proxy pages
# https://middlemanapp.com/advanced/dynamic-pages/

# Create language-specific proxies for each page with a 'link' in navigation data
LANGUAGES.each do |lang|
  data.navigation.Navigation.each do |page|
    next unless page.key?('link')

    # NOTE: changing this doesn't change localized_href(), breaking the overall program!!!!!!!!!
    src = page['link'][1..]
    dest = "/#{lang}_#{src[1..]}"
    proxy dest, src, locals: { lang: lang }, ignore: true
  end
end

# Helpers
# Methods defined in the helpers block are available in templates
# https://middlemanapp.com/basics/helper-methods/

# Helpers available in templates/partials
helpers do
  # Determine the current language from locals or URL path; fallback to first configured language
  def current_lang
    lang = current_page.locals[:lang]
    return lang if lang

    'en'
  end

  # Build a language-prefixed href from a base path like "./index.html" or "/research.html"
  # NOTE: changing this doesn't change proxy(), breaking the overall program!!!!!!!!!
  def localized_href(path, lang = nil)
    return path if path.nil? || path.strip.empty?

    lang ||= current_lang
    clean = path.sub(/^\./, '').sub(%r{^/}, '')
    "/#{lang}_#{clean}"
  end

  ## Return configured languages, client code must not modify the array!
  def config_langs
    LANGUAGES
  end
end

# Build-specific configuration
# https://middlemanapp.com/advanced/configuration/#environment-specific-settings

# configure :build do
#   activate :minify_css
#   activate :minify_javascript, compressor: Terser.new
# end

# Custom Integration, i.e mini-extension.
Dotenv.load('.env')
DEEPL_KEY = ENV['DEEPL_AUTH_KEY']
MAX_CHARS = 20_000 # max characters per API call

DeepL.configure do |config|
  config.auth_key = DEEPL_KEY
  # config.server_url = 'https://api.deepl.com' # default; use free endpoint if needed
end

require 'deepl'
require 'nokogiri'
require 'fileutils'

# Rubycop disabled due to criticality of method. Handles API calls for a limited API service.
def deepl_translate(text, is_html: true, target: 'EN') # rubocop:disable Metrics/AbcSize,Metrics/CyclomaticComplexity,Metrics/MethodLength
  # Skip API for English, when toggled off, or missing key
  return text if target == 'EN' || !TOGGLE_API || DEEPL_KEY.to_s.strip.empty?

  opts = {
    split_sentences: 'nonewlines',
    preserve_formatting: true
  }
  opts[:tag_handling] = 'html' if is_html

  result = DeepL.translate(text, 'EN', target, **opts)
  result.respond_to?(:text) ? result.text : Array(result).map(&:text).join
rescue StandardError => e
  logger.warn "DeepL translation failed (#{target}): #{e.class}: #{e.message}"
  text
end

# Batch-translate multiple plain-text strings while preserving 1:1 mapping. Rubycop disabled due to criticality of
# method and its criticality related to API calls.
def deepl_translate_many(texts, is_html: false, target: 'EN') # rubocop:disable Metrics/AbcSize,Metrics/CyclomaticComplexity,Metrics/MethodLength,Metrics/PerceivedComplexity
  # Fast skips
  return texts if target == 'EN' || !TOGGLE_API || DEEPL_KEY.to_s.strip.empty? || texts.empty?

  opts = {
    split_sentences: 'nonewlines',
    preserve_formatting: true
  }
  opts[:tag_handling] = 'html' if is_html

  if DeepL.respond_to?(:translate)
    res = DeepL.translate(texts, 'EN', target, **opts)
    Array(res).map { |r| r.respond_to?(:text) ? r.text : r.to_s }
  else
    # Fallback for SDKs without translate_text(arr): translate per item
    texts.map { |t| deepl_translate(t, is_html: is_html, target: target) }
  end
rescue StandardError => e
  logger.warn "DeepL batch translation failed (#{target}): #{e.class}: #{e.message}"
  texts
end

# Protect placeholders like {{variable}}, %s, {0} from being altered during translation
def protect_placeholders(str)
  placeholders = {}
  str = str.gsub(/\{\{.*?\}\}|%s|\{\d+\}/) do |m|
    key = "___PH#{placeholders.size}___"
    placeholders[key] = m
    key
  end
  [str, placeholders]
end

# Restore placeholders after translation
def restore_placeholders(str, placeholders)
  placeholders.each { |k, v| str = str.gsub(k, v) }
  str
end

# Gets all the text nodes that should be translated
def translatable_text_nodes(doc)
  nodes = []
  doc.traverse do |node|
    next unless node.text?

    parent = node.parent
    next if parent.nil?
    next if parent.ancestors('script, style, pre, code, template').any?

    next if node.text.strip.empty?

    nodes << node
  end
  nodes
end

# Map site language codes to DeepL target codes
LANG_TO_DEEPL = {
  'en' => 'EN',
  'de' => 'DE',
  'jp' => 'JA',
  'ja' => 'JA'
}

# Translate a single HTML file in place, this has some rubycop disabled due to complexity.
def translate_file(path) # rubocop:disable Metrics/AbcSize,Metrics/CyclomaticComplexity,Metrics/MethodLength,Metrics/PerceivedComplexity
  # Only handle language-prefixed outputs like "de_index.html", "ja_research.html"
  m = File.basename(path).match(/\A([a-z]{2})_.+\.html\z/i)
  return unless m

  lang2  = m[1].downcase
  target = LANG_TO_DEEPL[lang2] || lang2.upcase

  # Fast skip when translating English or when API is disabled/missing
  return if target == 'EN' || !TOGGLE_API || DEEPL_KEY.to_s.strip.empty?

  html =
    begin
      File.read(path, encoding: 'UTF-8')
    rescue StandardError => e
      warn "Read failed #{path}: #{e.class}: #{e.message}"
      return
    end

  doc =
    if defined?(Nokogiri::HTML5)
      Nokogiri::HTML5(html)
    else
      Nokogiri::HTML(html)
    end

  nodes = translatable_text_nodes(doc)

  # Translate text nodes in batches but preserve 1:1 mapping (no join/split).
  batch = []
  batch_len = 0
  flush_batch = lambda do
    next if batch.empty?

    # Extract texts and protect placeholders per item
    texts = []
    ph_maps = []
    batch.each do |node|
      safe, ph = protect_placeholders(node.text)
      texts << safe
      ph_maps << ph
    end

    # Translate as plain text (no HTML handling at node level)
    translated = deepl_translate_many(texts, is_html: false, target: target)

    # Restore placeholders and write back to original nodes
    batch.each_with_index do |node, i|
      node.content = restore_placeholders(translated[i] || '', ph_maps[i])
    end

    batch.clear
    batch_len = 0
  end

  nodes.each do |n|
    t = n.text
    flush_batch.call if batch_len + t.length > MAX_CHARS
    batch << n
    batch_len += t.length
  end
  flush_batch.call

  # Translate selected attributes in a similar 1:1 batched way
  attrs = %w[alt title aria-label placeholder]
  attr_elements = []
  attr_names    = []
  attr_texts    = []
  attr_ph_maps  = []

  attrs.each do |attr|
    doc.xpath("//*[@#{attr}]").each do |el|
      val = el[attr].to_s
      next if val.strip.empty?

      safe, ph = protect_placeholders(val)
      attr_elements << el
      attr_names << attr
      attr_texts << safe
      attr_ph_maps << ph
    end
  end

  attr_batch_size = 200 # reasonable chunking to keep request payloads small
  attr_texts.each_slice(attr_batch_size).with_index do |slice, slice_idx|
    tr = deepl_translate_many(slice, is_html: false, target: target)
    slice.each_with_index do |_, i|
      idx = slice_idx * attr_batch_size + i
      el  = attr_elements[idx]
      nm  = attr_names[idx]
      phm = attr_ph_maps[idx]
      el[nm] = restore_placeholders(tr[i] || '', phm)
    end
  end

  tmp = "#{path}.tmp"
  File.write(tmp, doc.to_html, encoding: 'UTF-8')
  FileUtils.mv(tmp, path)
end

# Run translation AFTER build output is written, this means server doesn't see translated files.
after_build do |_builder|
  out_dir = File.expand_path(config[:build_dir], app.root)
  Dir.glob(File.join(out_dir, '**', '*.html')).each do |file|
    # Only translate language-prefixed HTML files (e.g., "de_index.html")
    next unless File.basename(file) =~ /\A([a-z]{2})_.+\.html\z/i

    puts "Translating #{file}"
    begin
      translate_file(file)
    rescue StandardError => e
      warn "Failed #{file}: #{e.class}: #{e.message}"
    end
  end
end
