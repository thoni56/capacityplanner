#include "planner.h"

#include <hpdf.h>
#include <hpdf_types.h>

#include <setjmp.h>
#include <stdio.h>
#include <string.h>


static jmp_buf env;


static void error_handler(HPDF_STATUS error_no, HPDF_STATUS detail_no, void *user_data) {
    printf ("ERROR: error_no=%04X, detail_no=%d\n",
            (unsigned int) error_no, (int) detail_no);
    longjmp (env, 1); /* invoke longjmp() on error */
}


Project *create_project(char *project_name, int horisontal_divide, int vertical_divide) {
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
    project->horisontal_divide = horisontal_divide;
    project->vertical_divide = vertical_divide;
    project->fractions = 2;
    return project;
}

/* Sizes in pixels for A4 paper */
#define PAGE_WIDTH 841.89
#define PAGE_HEIGHT 595.267

void add_board(Project *project, int page_count) {
    for (int i = 0; i < page_count; i++) {
        HPDF_Page board_page = HPDF_AddPage(project->pdf);
        HPDF_Page_SetSize(board_page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_LANDSCAPE);
        HPDF_Page_SetRGBStroke(board_page, 0.3, 0.1, 0.7);
        HPDF_Page_SetRGBFill(board_page, 1, 1, 1);

        HPDF_REAL length = PAGE_WIDTH/project->horisontal_divide;
        HPDF_REAL height = PAGE_HEIGHT/project->vertical_divide;

        for (int row = 0; row < project->vertical_divide; row++)
            for (int column = 0; column < project->horisontal_divide; column++)
                HPDF_Page_Rectangle(board_page, column*length, row*height, length, height);
        HPDF_Page_ClosePathFillStroke(board_page);

        HPDF_UINT16 pattern[] = {3, 7};
        HPDF_Page_SetDash(board_page, pattern, 2, 0);
        HPDF_REAL fraction_height = height/project->fractions;
        for (int row = 0; row < project->vertical_divide; row++)
            for (int fraction = 1; fraction < project->fractions; fraction++) {
                HPDF_Page_MoveTo(board_page, 0, row*height+fraction*fraction_height);
                HPDF_Page_LineTo(board_page, PAGE_WIDTH, row*height+fraction*fraction_height);
                HPDF_Page_Stroke(board_page);
            }
    }
}


void add_feature(Project *project, char *feature_name, int height_in_fractions, int length) {
    HPDF_Page page = HPDF_AddPage(project->pdf);
    HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_LANDSCAPE);


}

void close_project(Project *project) {
    char *filename = strdup(project->name);
    filename = realloc(filename, strlen(filename) + strlen(".pdf") + 1);
    strcat(filename, ".pdf");
    HPDF_SaveToFile(project->pdf, filename);
}
