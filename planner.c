#include <setjmp.h>
#include <stdio.h>
#include <string.h>

#include "planner.h"
#include "hpdf.h"
#include "hpdf_types.h"


static jmp_buf env;


static void error_handler(HPDF_STATUS error_no, HPDF_STATUS detail_no, void *user_data) {
    printf ("ERROR: error_no=%04X, detail_no=%d\n",
            (unsigned int) error_no, (int) detail_no);
    longjmp (env, 1); /* invoke longjmp() on error */
}


Project *create_project(char *project_name) {
    HPDF_Doc pdf;
    Project *project;

    pdf = HPDF_New(error_handler, NULL);
    if (!pdf) {
        printf ("ERROR: cannot create pdf object.\n");
        return NULL;
    }

    if (setjmp(env)) {
        HPDF_Free(pdf);
        return NULL;
    }

    project = (Project*)malloc(sizeof(Project));
    project->name = project_name;
    project->pdf = pdf;
    return project;
}

void add_board(Project *project) {
}


void close_project(Project *project) {
    char *filename = strdup(project->name);
    filename = realloc(filename, strlen(filename) + strlen(".pdf") + 1);
    strcat(filename, ".pdf");
    HPDF_SaveToFile(project->pdf, filename);
}
