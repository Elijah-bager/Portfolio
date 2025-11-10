document.addEventListener('DOMContentLoaded', () => {
  const sidePanelCourses = document.querySelector('.side-panel-courses');
  const curriculumBody = document.getElementById('curriculum-body');

  // --- Event Delegation Setup ---
  // We attach listeners to the parent containers that are always present.
  // This ensures that even when course blocks are moved, the events still work.

  if (sidePanelCourses && curriculumBody) {
    // Drag and Drop Listeners
    document.body.addEventListener('dragstart', handleDragStart);
    document.body.addEventListener('dragover', handleDragOver);
    document.body.addEventListener('drop', handleDrop);

    // Hover Listeners
    document.body.addEventListener('mouseover', handleMouseOver);
    document.body.addEventListener('mouseout', handleMouseOut);
  }

  // --- Drag and Drop Handlers ---

  function handleDragStart(e) {
    if (e.target.classList.contains('course')) {
      e.dataTransfer.setData('text/plain', e.target.id);
      // Add a class to show it's being dragged
      setTimeout(() => e.target.classList.add('dragging'), 0);
    }
  }

  function handleDragOver(e) {
    e.preventDefault(); // Necessary to allow dropping
  }

  function handleDrop(e) {
    e.preventDefault();
    const courseId = e.dataTransfer.getData('text/plain');
    const draggedElement = document.getElementById(courseId);

    if (draggedElement) {
      draggedElement.classList.remove('dragging');

      const targetSemester = e.target.closest('td');
      const targetSidePanel = e.target.closest('.side-panel-courses');

      if (targetSemester) {
        // Dropped on a semester
        targetSemester.appendChild(draggedElement);
        draggedElement.style.width = 'auto';
        draggedElement.style.margin = '5px 0';
        draggedElement.classList.remove('missing-prereq'); // Remove highlight if it was missing
        checkPrerequisites(draggedElement);
      } else if (targetSidePanel) {
        // Dropped back into the side panel
        targetSidePanel.appendChild(draggedElement);
        draggedElement.style.width = ''; // Reverts to stylesheet's rule
        draggedElement.style.margin = '';
        // When moved back, re-evaluate if it's a missing prereq for courses in the planner
        updateAllMissingPrereqHighlights();
      }
    }
  }

  function checkPrerequisites(droppedCourse) {
    const prerequisites = droppedCourse.dataset.prerequisites;
    if (prerequisites) {
      const prereqIds = prerequisites.split(',');
      prereqIds.forEach(id => {
        if (id) {
          const prereqElement = document.getElementById(id);
          // Check if the prerequisite is still in the side panel
          if (prereqElement && prereqElement.closest('.side-panel-courses')) {
            prereqElement.classList.add('missing-prereq');
            // Move to top of side panel
            prereqElement.parentElement.prepend(prereqElement);
          }
        }
      });
    }
  }

  function updateAllMissingPrereqHighlights() {
    const sidePanel = document.querySelector('.side-panel-courses');
    const plannerCourses = document.querySelectorAll('#curriculum-table .course');
    
    // First, clear all existing missing-prereq highlights in the side panel
    sidePanel.querySelectorAll('.course').forEach(course => {
      course.classList.remove('missing-prereq');
    });

    // Then, re-check for every course in the planner
    plannerCourses.forEach(course => {
      checkPrerequisites(course);
    });
  }
  
  // --- Hover Handlers ---

  function handleMouseOver(e) {
    if (e.target.closest('.course')) {
      const currentCourse = e.target.closest('.course');
      currentCourse.classList.add('hovered');

      highlightPrerequisites(currentCourse, 'prereq-highlight');
      highlightPostrequisites(currentCourse, 'postreq-highlight');
    }
  }

  function handleMouseOut(e) {
    if (e.target.closest('.course')) {
      const currentCourse = e.target.closest('.course');
      currentCourse.classList.remove('hovered');

      // Clear all highlights
      const allCourses = document.querySelectorAll('.course');
      allCourses.forEach(course => {
        course.classList.remove(
          'prereq-highlight', 
          'prereq-highlight-secondary',
          'postreq-highlight',
          'postreq-highlight-secondary'
        );
      });
    }
  }

  function highlightPrerequisites(courseElement, highlightClass) {
    const prerequisites = courseElement.dataset.prerequisites;
    if (prerequisites) {
      const prereqIds = prerequisites.split(',');
      prereqIds.forEach(id => {
        if (id) {
          const prereqElements = document.querySelectorAll(`#${id}`);
          prereqElements.forEach(el => {
            el.classList.add(highlightClass);
            // Recursively highlight the next level of prerequisites
            highlightPrerequisites(el, 'prereq-highlight-secondary');
          });
        }
      });
    }
  }

  function highlightPostrequisites(courseElement, highlightClass) {
    const postrequisites = courseElement.dataset.postrequisites;
    if (postrequisites) {
      const postreqIds = postrequisites.split(',');
      postreqIds.forEach(id => {
        if (id) {
          const postreqElements = document.querySelectorAll(`#${id}`);
          postreqElements.forEach(el => {
            el.classList.add(highlightClass);
            // Recursively highlight the next level of post-requisites
            highlightPostrequisites(el, 'postreq-highlight-secondary');
          });
        }
      });
    }
  }

  // --- Initial Load ---
  if (sidePanelCourses && curriculumBody) {
    updateAllMissingPrereqHighlights();
    // ... rest of the event listeners
  }
});
