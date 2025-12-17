# proj4-elden-ring

Faculty page / small site template built with Middleman.

This repository is a Middleman-based static site scaffolding used for a faculty web page (navigation, pages, footer, and optional server-side translations). It includes a translater located in `config.rb`.

## Features

- Middleman 4 site structure (templates in `source/`, data in `data/`).
- Navigation driven by `data/navigation.yml`.
- Footer data in `data/footer.json` and rendered from `source/_footer.erb`.
- Fixed header navigation and a flexbox footer that stays at the bottom on short pages.
- Optional DeepL translation integration (configured in `config.rb`).

## Requirements

- Ruby (2.7+ recommended) and Bundler
- Node.js is not strictly required for this project but may be useful for frontend tooling.

## Setup

1. Install dependencies:

```bash
bundle install
```

2. Start the Middleman development server:

```bash
bundle exec middleman server
```

Note: LocalHost will open onto /index.html, which is ignored, type in .../en_index.html

Open <http://localhost:4567> to preview the site.

## Build

To generate a static build into the `build/` directory:

```bash
bundle exec middleman build
```

## Notes on translations

- Translations: `config.rb` contains DeepL integration and language proxy configuration. Set `DEEPL_AUTH_KEY` in your `.env` file and toggle `TOGGLE_API` in `config.rb` to enable API translations. See `config.rb` for details on language proxies and helpers such as `localized_href`.
Note that this feature works only for middleman builds and not server previews unless changing line 284 from `after_build` to `ready`, but this is unintended behavior.

## Project structure (important files)

- `config.rb` — Middleman configuration, language proxies, and translation hooks.
- `data/navigation.yml` — Navigation items (title, link) used by `source/_navigation.html.erb`.
- `data/footer.json` — Footer contact/name and external links used by `source/_footer.erb`.
- `source/layouts/layout.erb` — The main layout; includes stylesheets and renders navigation/footer partials.
- `source/_navigation.html.erb` — Navigation partial (includes the search form).
- `source/_footer.erb` — Footer partial (now wrapped in a `<footer>` element).
- `source/stylesheets/` — CSS files including `site.css.scss`, `navigation_style.css`, and `footer_style.css`.
- `source/javascripts/site.js` — Small site JS (optional; search is implemented server-side in this repo by default).

## Maintainability (brief)

- `Adding pages` -- New pages can be added into source as new \*.html\*.erb files and making sure to add them into `data/navigation.yml`
- `Affecting Translations` -- All pages in the `navigation.yml` file are dynamically generated as localized pages. Changing which languages to use can be done in `config.rb` with the LANGUAGES constant, please check `deepl` ruby documenation for available languages.
- `API toggle and keys` -- DeepL API calls can be toggled on and off for building by changing the API_TOGGLE constant in `config.rb`. See `Notes on translations` above.
