require 'minitest/autorun'
require_relative '../lib/fetch_carmen_data'

##
# Testing fetch_carmen_data
# Suggested viewing involves condensing constants to single line in vscode
class Testcarmen < Minitest::Test # rubocop:disable Metrics/ClassLength
  NO_AUTH = '{"status":"unauthenticated","errors":[{"message":"user authorization required"}]}'.freeze
  NO_AUTH_KEYWORD = 'unauthenticated'.freeze

  CARMEN_COURSE_EXAMPLE = [{
    id: 191_100,
    name: 'AU25 COMM 3325 - Intro Org Comm (16323)',
    account_id: 118,
    uuid: 'lmax05QgrfC3nML8SxnS7HVeAdXprho9y89Nfxk7',
    start_at: null,
    grading_standard_id: 1585,
    is_public: false,
    created_at: '2025-08-05T15:27:14Z',
    course_code: 'COMM 3325 AU2025 (16323)',
    default_view: 'modules',
    root_account_id: 1,
    enrollment_term_id: 272,
    license: 'private',
    grade_passback_setting: null,
    end_at: null,
    public_syllabus: false,
    public_syllabus_to_auth: false,
    storage_quota_mb: 786,
    is_public_to_auth_users: false,
    homeroom_course: false,
    course_color: null,
    friendly_name: null,
    apply_assignment_group_weights: false,
    calendar: {
      ics: 'REMOVED'
    },
    time_zone: 'America/New_York',
    blueprint: false,
    template: false,
    enrollments: [
      {
        type: 'student',
        role: 'StudentEnrollment',
        role_id: 3,
        user_id: 1_315_826,
        enrollment_state: 'active',
        limit_privileges_to_course_section: false
      }
    ],
    hide_final_grades: false,
    workflow_state: 'available',
    restrict_enrollments_to_course_dates: false
  }].to_json.freeze

  CARMEN_ASSIGNMENT_EXAMPLE = [{
    id: 4_788_156,
    description: "\u003Cul\u003E\n\u003Cli\u003EHow important is it for you to be heard in your own work or organizations? How does it change what you do when you are or are not heard?\u003C/li\u003E\n\u003Cli\u003EFind examples of Human Relations in your organization. How do they apply these ideas? Are they different from their main competitor(s)?\u003C/li\u003E\n\u003C/ul\u003E\n\u003Cp\u003E&nbsp;\u003C/p\u003E\n\u003Cp\u003EInitial post due Friday 11:59pm, 2 quality responses due Tuesday 11:59pm.&nbsp;\u003C/p\u003E",
    due_at: null,
    unlock_at: '2025-10-04T04:00:00Z',
    lock_at: '2025-10-15T03:59:59Z',
    points_possible: 10,
    grading_type: 'points',
    assignment_group_id: 847_523,
    grading_standard_id: null,
    created_at: '2025-08-05T15:29:47Z',
    updated_at: '2025-10-04T04:00:08Z',
    peer_reviews: false,
    automatic_peer_reviews: false,
    position: 1,
    grade_group_students_individually: false,
    anonymous_peer_reviews: false,
    group_category_id: null,
    post_to_sis: false,
    moderated_grading: false,
    omit_from_final_grade: false,
    intra_group_peer_reviews: false,
    anonymous_instructor_annotations: false,
    anonymous_grading: false,
    graders_anonymous_to_graders: false,
    grader_count: 0,
    grader_comments_visible_to_graders: true,
    final_grader_id: null,
    grader_names_visible_to_final_grader: true,
    allowed_attempts: -1,
    lock_info: {
      lock_at: '2025-10-15T03:59:59Z',
      can_view: true,
      asset_string: 'assignment_4788156'
    },
    course_id: 191_100,
    name: 'Human Relations Discussion',
    submission_types: [
      'discussion_topic'
    ],
    has_submitted_submissions: true,
    due_date_required: false,
    max_name_length: 255,
    in_closed_grading_period: false,
    graded_submissions_exist: false,
    is_quiz_assignment: false,
    can_duplicate: true
  }].to_json.freeze

  CARMEN_ANNOUCEMENT_EXAMPLE_1 = [{
    id: 1,
    title: 'Hear ye',
    message: 'Henceforth, all assignments must be... class cancelled',
    posted_at: '2017-01-31T22:00:00Z',
    delayed_post_at: null,
    context_code: 'course_2'
  }].to_json.freeze

  CARMEN_ANNOUCEMENT_EXAMPLE_2 = [{
    id: 2,
    title: 'Hear ye',
    message: 'Henceforth, all assignments must be... important',
    posted_at: '2017-01-31T22:00:00Z',
    delayed_post_at: null,
    context_code: 'course_2'
  }].to_json.freeze

  CARMEN_ANNOUCEMENT_EXAMPLE_3 = [{
    id: 3,
    title: 'Hear ye, class cancelled',
    message: 'Henceforth, all assignments must be... important',
    posted_at: '2017-01-31T22:00:00Z',
    delayed_post_at: null,
    context_code: 'course_2'
  }].to_json.freeze

  # Sets up the static values grabbed from json constants
  def setup # rubocop:disable Metrics/MethodLength,Metrics/AbcSize
    # fill in the fields for testing Course object based on CARMEN_COURSE_EXAMPLE
    course_example_data = JSON.parse(CARMEN_COURSE_EXAMPLE)[0]
    date_str = course_example_data['created_at']
    date = DateTime.parse(date_str) # Will Parse ISO 8601 format used in created_at
    @carmen_course_day = date.day
    @carmen_course_month = date.month
    @carmen_course_year = date.year
    @carmen_course_id = course_example_data['id']
    @carmen_course_name = course_example_data['name']
    @carmen_course_code = course_example_data['course_code']

    assignment_example_data = JSON.parse(CARMEN_ASSIGNMENT_EXAMPLE)[0]
    @carmen_assignment_id = assignment_example_data['id']
    @carmen_assignment_name = assignment_example_data['name']
    @carmen_assignment_ddate = 'No due date'
    @carmen_assignment_points_possible = assignment_example_data['points_possible']
    @carmen_assignment_submitted = assignment_example_data['has_submitted_submissions']
    @carmen_assignment_graded = assignment_example_data['graded_submissions_exist']
    @caremn_assignment_course_id = assignment_example_data['course_id']
    @static_assignment = CarmenData::Assignment.new(assignment_example_data)
  end

  ## testing based on the assumption that requests gave good data, so force good data be given to methods under test ##

  # Testing if auth code bearer is working, use is mainly for debugging and not validation
  def test_carmen_fetch
    req = CarmenData.carmen_fetch(CarmenData::COURSES_URL)
    json = JSON.parse(req.body.to_s)
    assert_false json.to_s.include?(NO_AUTH_KEYWORD),
                 'Carmen fetch failed on authorization, if failed, check README and .env setup'
  end

  # Can not test Courses.refresh! or Courses#initialize as they depend on carmen_fetch,
  # which can not be guaranteed to work in test environment. So, test Courses.populate_from_json!
  # directly with good data.
  def test_courses_populate_from_json
    courses = CarmenData::Courses.new
    courses.clear # Clear out the initial fetch attempt in initialize (which may have failed due to no auth, so don't rely on its data)
    courses.populate_from_json!(JSON.parse(CARMEN_COURSE_EXAMPLE))
    # due to how the module is structured (and its classes), courses will still try to populate assignments and announcements
    # from live data requests, which is beyond the scope of this test, so just check that we got one course object and ignore its sub-objects
    assert_instance_of CarmenData::Course, courses[0]
    assert_equal 1, courses.length
  end

  # test if the processing of the course creation date is correct
  def test_course_process_creation_date
    course_data = JSON.parse(CARMEN_COURSE_EXAMPLE)[0]
    course = CarmenData::Course.new(course_data)
    assert_equal @carmen_course_day, course.day
    assert_equal @carmen_course_month, course.month
    assert_equal @carmen_course_year, course.year
  end

  # test if a course initialization is correct from static json, doesn't check its
  # assignments or annoucements beyond there being array declared to hold them.
  def test_course_initialization
    course_data = JSON.parse(CARMEN_COURSE_EXAMPLE)[0]
    course = CarmenData::Course.new(course_data)
    assert_equal @carmen_course_id, course.id
    assert_equal @carmen_course_name, course.name
    assert_equal @carmen_course_code, course.course_code
    # assignments and announcements are fetched live, so just check they are arrays
    assert_instance_of Array, course.assignments
    assert_instance_of Array, course.announcements
  end

  # test if assignment initialization from a static json object done correctly
  def test_assignment_initialization # rubocop:disable Metrics/AbcSize
    assignment_data = JSON.parse(CARMEN_ASSIGNMENT_EXAMPLE)[0]
    assignment = CarmenData::Assignment.new(assignment_data)
    assert_equal @caremn_assignment_course_id, assignment.course_id
    assert_equal @carmen_assignment_graded, assignment.graded
    assert_equal @carmen_assignment_submitted, assignment.submitted
    assert_equal @carmen_assignment_points_possible, assignment.points_possible
    assert_equal @carmen_assignment_name, assignment.as_name
    assert_equal @carmen_assignment_id, assignment.as_id
    assert_equal @carmen_assignment_ddate, assignment.due_at
  end

  # test if the parsing of due date for assignment handles null cases
  def test_parse_due_date_null
    due_null = @static_assignment.parse_due_date(null)
    assert_equal 'No due date', due_null
  end

  # test if the parsing of due date for assignment is done correctly for given ISO date
  def test_parse_due_date_not_null
    due_not_null = @static_assignment.parse_due_date('2025-08-05T15:27:14Z')
    date = DateTime.parse('2025-08-05T15:27:14Z')
    assert_equal date.strftime('%m/%d/%Y %H:%M:%S %Z'), due_not_null
  end

  # tests the initialization of an annoucement is done correctly from a EXAMPLE annoucement json
  def test_annoucement_initialization
    annoucement_data = JSON.parse(CARMEN_ANNOUCEMENT_EXAMPLE_1)[0]
    annoucement = CarmenData::Announcement.new(annoucement_data)
    assert_equal 1, annoucement.id
    assert_equal 'Hear ye', annoucement.title
    assert_equal 'Henceforth, all assignments must be... class cancelled', annoucement.message
  end

  # test if the parsing of the posted date is done correctly for a given ISO date
  def test_parse_posted_date
    annoucement_data = JSON.parse(CARMEN_ANNOUCEMENT_EXAMPLE_1)[0]
    annoucement = CarmenData::Announcement.new(annoucement_data)
    test_date = annoucement.parse_posted_date('2025-08-05T15:27:14Z')
    date = DateTime.parse('2025-08-05T15:27:14Z')
    assert_equal date.strftime('%m/%d/%Y %H:%M:%S %Z'), test_date
  end

  # test if an annoucement object contains keyword class cancelled, then that this is true
  def test_class_cancelled_true
    annoucement_data = JSON.parse(CARMEN_ANNOUCEMENT_EXAMPLE_1)[0]
    annoucement = CarmenData::Announcement.new(annoucement_data)
    assert_equal true, annoucement.class_cancelled?
  end

  # test if an annoucement object doesn't contains keyword class cancelled, then that this is false
  def test_class_cancelled_false
    annoucement_data = JSON.parse(CARMEN_ANNOUCEMENT_EXAMPLE_2)[0]
    annoucement = CarmenData::Announcement.new(annoucement_data)
    assert_equal false, annoucement.class_cancelled?
  end

  # test if an annoucement object contains keyword important, then that this is true
  def test_important_true
    annoucement_data = JSON.parse(CARMEN_ANNOUCEMENT_EXAMPLE_2)[0]
    annoucement = CarmenData::Announcement.new(annoucement_data)
    assert_equal true, annoucement.important?
  end

  # test if an annoucement object doesn't contains keyword important, then that this is false
  def test_important_false
    annoucement_data = JSON.parse(CARMEN_ANNOUCEMENT_EXAMPLE_1)[0]
    annoucement = CarmenData::Announcement.new(annoucement_data)
    assert_equal true, annoucement.important?
  end

  # test if an annoucement object contains both keywords, then that both are true.
  def test_class_cancelled_true_with_important_true
    annoucement_data = JSON.parse(CARMEN_ANNOUCEMENT_EXAMPLE_3)[0]
    annoucement = CarmenData::Announcement.new(annoucement_data)
    assert_equal true, annoucement.class_cancelled?
    assert_equal true, annoucement.important?
  end
end
