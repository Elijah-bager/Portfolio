# Elden Ring Curriculum Planner

This project is an interactive curriculum planner designed to help students visualize and organize their four-year academic plan. It is built using the Middleman static site generator and features a dynamic, drag-and-drop interface for course scheduling. While some additional features were not implemented, the student user can use this tool in addition to their
Major and Minor supplied course and other requirements to create a color linked curriculmn schedule. The current courses and
majors are for Ohio State University and specifically to help creating a CIS course, this is just an example and different
courses can be added / used by editing the courses.yml file.

## Features

**Interactive Planner Grid**: A 4-year (8-semester) grid where users can place courses.

**Course Side Panel**: A scrollable and searchable list of all available courses that can be dragged into the planner.

**Drag-and-Drop Functionality**: Easily move courses from the side panel into any semester in the planner grid.

**Prerequisite & Post-requisite Highlighting**:
    *Hover over a course to instantly highlight its direct prerequisites in yellow and post-requisites in cyan.
    *The highlighting extends to secondary prerequisites (light yellow) and post-requisites (light blue), allowing for full chain visibility.

**Missing Prerequisite Alerts**: When a course is added to the planner, the system automatically checks if its prerequisites have been scheduled. If not, the missing prerequisite courses are highlighted in orange and moved to the top of the side panel for easy identification.

## Project layout

- source/
  - javascripts/ — client logic (drag/drop, highlighting)
  - stylesheets/ — CSS for layout and color keys
  - images/ — background and other assets
  - index.html.erb — main page
- data/
  - courses.yml — courses, credits, prereqs
  - majors.yml — list of majors (YAML)
- build/ — generated static site (after build)


## How to Set Up and Run the Project

This project is built with Middleman, a Ruby-based static site generator.

### Prerequisites

**Ruby**: Ensure you have Ruby installed on your system.
**Bundler**: If you don't have Bundler, install it with `gem install bundler`.

### Installation & Setup

1.**Clone the repository:**
    ```bash
    git clone <repository-url>
    cd proj5-elden-ring
    ```

2.**Install dependencies:**
    Run the following command to install the required gems from the `Gemfile`.
    ```bash
    bundle install
    ```

3.**Run the local server:**
    Start the Middleman server to view the site locally.
    ```bash
    bundle exec middleman server
    ```

4.**View the project:**
    Open your web browser and navigate to `http://localhost:4567` to see the application in action.

## Important Notes

**Course Data**: All course information (name, credits, prerequisites) is managed in the `data/courses.yml` file. To add, remove, or modify courses, edit this YAML file. The application will automatically update. Use the courses already there for
reference examples on how insert new courses.
**Interaction Logic**: The core client-side functionality (drag-and-drop, highlighting) is handled by the JavaScript file located at `source/javascripts/interactions.js`.
**Styling**: Styles, including the colors for highlighting, are defined in the CSS files within the `source/stylesheets/` directory.
