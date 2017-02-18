#include <cgreen/cgreen.h>

#include "planner.h"

Describe(Planner);
BeforeEach(Planner) {}
AfterEach(Planner) {}

Ensure(Planner, can_create_empty_project_with_correct_name) {
    Project *project = create_project("project", 2, 4);

    assert_that(project->name, is_equal_to_string("project"));
}

Ensure(Planner, can_create_empty_project_and_pdf) {
    Project *project = create_project("project", 2, 4);
    close_project(project);

    assert_that(fopen("project.pdf", "r"), is_not_null);
}

Ensure(Planner, can_create_project_and_add_a_planboard) {
    Project *project = create_project("project", 3, 5);
    add_board(project, 2);
    close_project(project);

    FILE *pdf = fopen("project.pdf", "r");
}

Ensure(Planner, can_create_project_and_add_feature) {
    Project *project = create_project("project", 3, 5);
    add_board(project, 2);
    add_feature(project, "feature1", 2, 2);
    close_project(project);

    FILE *pdf = fopen("project.pdf", "r");
}
