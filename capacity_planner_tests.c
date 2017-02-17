#include <cgreen/cgreen.h>

#include "planner.h"

Describe(CapacityPlanner);
BeforeEach(CapacityPlanner) {}
AfterEach(CapacityPlanner) {}

Ensure(CapacityPlanner, can_create_empty_project_with_correct_name) {
    Project *project = create_project("project");

    assert_that(project->name, is_equal_to_string("project"));
}

Ensure(CapacityPlanner, can_create_empty_project_and_an_empty_document) {
    Project *project = create_project("project");
    close_project(project);

    assert_that(fopen("project.pdf", "r"), is_not_null);
}


Ensure(CapacityPlanner, can_create_project_and_add_a_planboard) {
    Project *project = create_project("project");
    add_board(project);
    close_project(project);
    
    FILE *pdf = fopen("project.pdf", "r");
}
