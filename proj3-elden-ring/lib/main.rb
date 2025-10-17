# frozen_string_literal: true

# Main entry point for dashboard and email functionality
# Loads course, assignment, announcement, and weather data, renders HTML, and sends email

require_relative 'fetch_carmen_data'
require_relative 'fetch_weather_data'
require 'erb'
require 'mail'
require 'dotenv'
require 'net/http'

Dotenv.load('.env')

# Get course data from Canvas API
@courses = CarmenData::Courses.new
@courses.refresh!

# Collect assignments and announcements from all courses
@assignments = []
@announcements = []

@courses.each do |course|
  course.get_assignments! if course.respond_to?(:get_assignments!)
  course.get_announcements! if course.respond_to?(:get_announcements!)
  @assignments.concat(course.assignments) if course.respond_to?(:assignments) && course.assignments
  @announcements.concat(course.annoucements) if course.respond_to?(:annoucements) && course.annoucements
end

# Get weather data (assuming fetch_weather returns a hash)
weather = fetch_weather('Columbus, Ohio')
@temperature = weather[:temperature]
@weather_status = weather[:weather_status]
@city = 'Columbus'
@state = 'Ohio'
# Render the template
template = File.read('lib/index.erb')
renderer = ERB.new(template)
html = renderer.result(binding)
File.write('index.html', html)

# Sending out the email

# Fetch a random quote from ZenQuotes API
def fetch_random_quote
  url = URI('https://zenquotes.io/api/random')
  response = Net::HTTP.get(url)
  data = JSON.parse(response)
  quote = data[0]['q']
  author = data[0]['a']
  "\"#{quote}\" - #{author}"
end

options = {
  address: 'smtp.gmail.com',
  port: 587,
  user_name: 'carmen.canvasupdates@gmail.com',
  password: 'yrjl nbvl wqfd kzqx',
  authentication: 'plain',
  enable_starttls_auto: true
}

Mail.defaults do
  delivery_method :smtp, options
end

# Send the dashboard and weather update email
Mail.deliver do
  from 'carmen.canvasupdates@gmail.com'
  to ENV.fetch('USER_EMAIL', nil)
  subject fetch_random_quote # Use random quote as subject
  html_part do
    content_type 'text/html; charset=UTF-8'
    body html # Use rendered HTML as email body
  end
end
