#include <cgreen/cgreen.h>

#include "planner.h"

/* Since it is very hard to inspect a PDF document programatically,
   I decided to just open the generated PDF with evince. Since evince
   automatically updates on file changes, you get a visual verification
   at least. Not ideal, but for this quick hack, good enough... */

Describe(Planner);
BeforeEach(Planner) {}
AfterEach(Planner) {}

Ensure(Planner, can_create_empty_project_with_correct_name) {
    Project *project = create_project("project");

    assert_that(project->name, is_equal_to_string("project"));
}

Ensure(Planner, can_create_empty_project_and_pdf) {
    Project *project = create_project("project");
    close_project(project);

    assert_that(fopen("project.pdf", "r"), is_not_null);
}

Ensure(Planner, can_create_project_and_add_a_planboard) {
    Project *project = create_project("project");
    add_board(project, 2);
    close_project(project);
}


Ensure(Planner, can_create_project_and_add_feature) {
    Project *project = create_project("project");
    add_board(project, 2);
    add_feature(project, "feature1", 2, 2);
    close_project(project);
}


Ensure(Planner, can_create_project_and_add_two_features) {
    Project *project = create_project("project");
    set_horizontal_divide(project, 3);
    set_vertical_divide(project, 5);
    add_board(project, 2);
    add_feature(project, "feature1", 2, 2);
    add_feature(project, "feature2", 4, 3);
    close_project(project);
}

Ensure(Planner, can_create_project_and_select_features_to_fill_height) {
    Project *project = create_project("project");
    set_horizontal_divide(project, 3);
    set_vertical_divide(project, 5);
    set_fractions(project, 2);

    add_board(project, 2);

    add_feature(project, "feature1", 2, 2);
    add_feature(project, "feature2", 4, 3);
    add_feature(project, "feature3", 5, 3);
    add_feature(project, "feature4", 4, 13);

    close_project(project);
}
