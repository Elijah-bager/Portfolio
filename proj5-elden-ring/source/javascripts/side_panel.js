function searchCourses() {
  let input, filter, courses, course, h3, i, txtValue;
  input = document.getElementById('course-search');
  filter = input.value.toUpperCase();
  courses = document.querySelector('.side-panel-courses').getElementsByClassName('course');

  for (i = 0; i < courses.length; i++) {
    course = courses[i];
    h3 = course.getElementsByTagName("h3")[0];
    txtValue = h3.textContent || h3.innerText;
    if (txtValue.toUpperCase().indexOf(filter) > -1) {
      course.style.display = "";
    } else {
      course.style.display = "none";
    }
  }
}
