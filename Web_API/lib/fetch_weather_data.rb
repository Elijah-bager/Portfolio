# frozen_string_literal: true

require 'httpx'
require 'json'
require 'dotenv'
require 'erb'

Dotenv.load('.env')

# Set up constants for API access
API_KEY = ENV.fetch('WEATHERSTACK_API_KEY', nil) # Your Weatherstack API key
CITY = 'Columbus, Ohio'.freeze                   # City to fetch weather for
BASE_URL = 'http://api.weatherstack.com/current'.freeze # Weatherstack API endpoint

# Fetches weather data for a given city from Weatherstack API
# Returns a hash with temperature and weather status
def fetch_weather(city)
  # Make GET request to Weatherstack API with city and API key
  response = HTTPX.get(BASE_URL, params: {
                         access_key: API_KEY,
                         query: city,
                         units: 'f' # Request temperature in Fahrenheit
                       })

  # Parse the JSON response
  data = JSON.parse(response.to_s)

  # Extract temperature and weather description
  temperature = data['current']['temperature']
  weather_status = data['current']['weather_descriptions'].join(', ')

  # Return weather info as a hash
  { temperature: temperature, weather_status: weather_status }
end

fetch_weather(CITY)
