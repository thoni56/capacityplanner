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
    //add_feature(project, "feature2", 4, 3);
    close_project(project);

    FILE *pdf = fopen("project.pdf", "r");
}
