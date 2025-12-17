# frozen_string_literal: true

#
# CarmenData: Module for fetching and organizing course, assignment, and announcement data from OSU Canvas API.
# Uses HTTPX for API requests, parses JSON, and structures data into Ruby objects for use in dashboards, emails, etc.

require 'dotenv'
require 'json'
require 'httpx'
require 'erb'
require 'date'

## CarmenData module
# Contains:
# - Constants for API URLS for requesting data
# - Function: carmen_fetch - given api url, adds a auth & json header and
# then HTTPX requests from api, returning request.
# - Classes:
#   - Courses, an extension of Arrays that focuses on getting an array of Course Objects
#   - Course, a single Course containing information related to that course extracted from json
#   and arrays of Assignments & Annoucements that come from the course.
#   - Assignment, a single Assignment object containing related information about the Assignment
#   - Annoucement, a single Annoucement object containing related information and methods to get slightly
#   processed information.
module CarmenData
  # Fetches data from Canvas API using provided URL and authentication token
  # Returns HTTPX response object
  def self.carmen_fetch(url)
    Dotenv.load('.env')
    token = ENV.fetch('CANVAS_TOKEN', nil)
    auth = "Bearer #{token}"
    rep = HTTPX.with(headers: { 'Authorization' => auth, 'Content-Type' => 'application/json' })
    rep.get(url)
  end

  ## Constants for API URLS, single point of change, don't change please ##
  # These URLs are used to fetch courses, assignments, and announcements for the authenticated user
  # API URL for fetching active courses where student is enrolled (student being one that has the token loaded)
  COURSES_URL = 'https://osu.instructure.com/api/v1/courses?enrollment_type=student&enrollment_state=active'
  ASSIGNMENTS_URL = 'https://osu.instructure.com/api/v1/courses/%<course_id>s/assignments'
  ANNOUNCEMENTS_URL = 'https://osu.instructure.com/api/v1/courses/%<course_id>s/discussion_topics?only_announcements=true'

  # Class for course array, implements iterable via array methods, so just 'extend Array'
  # Courses: Array-like class holding Course objects for a user's active courses
  class Courses < Array
    # NOTE! This class uses Array's self for holding Course Objects

    def initialize
      super # call Array's initialize
      refresh! # populate from Canvas API on init
    end

    # Populates the Courses array from JSON data returned by the API
    def populate_from_json!(json)
      json.each do |course_data|
        self << Course.new(course_data)
      end
      # Optionally check for current term (stubbed)
    end

    # Refreshes the Courses array by fetching latest data from Canvas API
    def refresh!
      req = CarmenData.carmen_fetch(COURSES_URL)
      json = JSON.parse(req.body.to_s)
      puts(json) # Debug: print raw JSON
      clear
      populate_from_json!(json)
    end
  end

  ## Course Object
  # A single course with its name, id, course_code, creation day-month-year.
  # Contains an array of Assignment and Annoucement Objects associated with the course.
  class Course
    # Fields of course object information # Parsed date fields from created_at, used to determine if current term.
    # id: Canvas course ID
    # name: Course name
    # course_code: Course code (e.g., CSE 3421 AU2025)
    # day, month, year: Parsed from created_at
    attr_reader :id, :name, :course_code, :day, :month, :year
    attr_accessor :assignments, :annoucements # Arrays of sub-objects

    # Makes a new Course object from a json containing a single course.
    def initialize(course_data)
      @id = course_data['id'] # Canvas course ID
      @name = course_data['name'] # Course name
      @course_code = course_data['course_code'] # Course code
      process_creation_date!(course_data['created_at']) # Parse creation date
      @assignments = [] # Array of Assignment objects
      @announcements = [] # Array of Announcement objects
      get_assignments! # Fetch assignments for this course
      get_announcements! # Fetch announcements for this course
    end

    # Parses ISO 8601 date string and sets day, month, year
    def process_creation_date!(date_str)
      date = DateTime.parse(date_str)
      @day = date.day
      @month = date.month
      @year = date.year
    end

    # Requests from API the assignments associated with this course_id and creates an array of Assignment objects
    def get_assignments!
      req = CarmenData.carmen_fetch(format(ASSIGNMENTS_URL, course_id: @id).to_s)
      json = JSON.parse(req.body.to_s)
      @assignments.clear
      json.each do |assignment_data|
        @assignments << Assignment.new(assignment_data)
      end
    end

    # Requests from API the annoucements associated with this course_id and creates an array of Annoucement objects
    def get_announcements!
      req = CarmenData.carmen_fetch(format(ANNOUNCEMENTS_URL, course_id: @id).to_s)
      json = JSON.parse(req.body.to_s)
      @announcements.clear
      json.each do |announcement_data|
        @announcements << Announcement.new(announcement_data)
      end
    end
  end

  ## Annoucement Object
  # A single Annoucement Object containing the id, title, message, and posted_at
  # Also has methods that primitively check if title or message contain stuff about cancellation or importance
  class Announcement
    attr_reader :id, :title, :message, :posted_at

    # Creates a new Annoucement Object from a json containing a single annoucement
    def initialize(announcement_data)
      @id = announcement_data['id']
      @title = announcement_data['title']
      @message = announcement_data['message']
      @posted_at = parse_posted_date(announcement_data['posted_at'])
    end

    # Parses posted date string, returns formatted date or fallback
    def parse_posted_date(posted_date_str)
      return 'No posted date' if posted_date_str.nil?

      date = DateTime.parse(posted_date_str)
      date.strftime('%m/%d/%Y %H:%M:%S %Z')
    end

    # Returns true if announcement is about class cancellation, but false doesn't mean no cancellation
    def class_cancelled?
      @title.downcase.include?('class cancelled') || @message.downcase.include?('class cancelled')
    end

    # Returns true if announcement is marked important, but false doesn't mean not important
    def important?
      @title.downcase.include?('important') || @message.downcase.include?('important')
    end
  end

  ## Assignment: Represents a single course assignment
  # contains information for the assignment id, name, due date (if applicable), points, submission status,
  # if graded, and the relative course_id
  class Assignment
    attr_reader :as_id, :as_name, :due_at, :points_possible, :submitted, :graded, :course_id

    # Initializes an Assignment object from API data
    def initialize(assignment_data)
      @as_id = assignment_data['id']
      @as_name = assignment_data['name']
      # due_at may be null, but parse will check.
      @due_at = parse_due_date(assignment_data['due_at'])
      @points_possible = assignment_data['points_possible']
      @submitted = assignment_data['has_submitted_submissions']
      @graded = assignment_data['graded_submissions_exist']
      @course_id = assignment_data['course_id']
    end

    # Parses due date string, returns formatted date or fallback
    def parse_due_date(due_date_str)
      return 'No due date' if due_date_str.nil?

      date = DateTime.parse(due_date_str)
      date.strftime('%m/%d/%Y %H:%M:%S %Z')
    end
  end
end
