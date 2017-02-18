#include "planner.h"

#include <hpdf.h>
#include <hpdf_types.h>

#include <setjmp.h>
#include <stdio.h>
#include <string.h>

#include "pdf.h"


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
    RGB stroke_color = {0,0,0};
    RGB fill_color = {255,255,255};

    for (int i = 0; i < page_count; i++) {
        Page page = add_page(project->pdf);
        set_stroke(page, BLACK);
        set_fill(page, WHITE);

        float length = PAGE_WIDTH/project->horisontal_divide;
        float height = PAGE_HEIGHT/project->vertical_divide;

        for (int row = 0; row < project->vertical_divide; row++)
            for (int column = 0; column < project->horisontal_divide; column++) {
                Position pos = {column*length, row*height};
                draw_rectangle(page, pos, length, height);
            }

        DashPattern dash_pattern = {2, {3,7}};
        set_dash(page, dash_pattern);
        float fraction_height = height/project->fractions;
        for (int row = 0; row < project->vertical_divide; row++)
            for (int fraction = 1; fraction < project->fractions; fraction++) {
                Position start = {0, row*height+fraction*fraction_height};
                Position end = {PAGE_WIDTH, row*height+fraction*fraction_height};
                draw_line(page, start, end);
            }
    }
}


void add_feature(Project *project, char *feature_name, int height_in_fractions, int length_in_sprints) {
    Page page = add_page(project->pdf);

    HPDF_REAL width = PAGE_WIDTH/project->horisontal_divide*length_in_sprints;
    HPDF_REAL height = PAGE_HEIGHT/project->vertical_divide/project->fractions*height_in_fractions;

    RGB fill_color = {190, 190, 190};
    Position at = {0,0};
    set_fill(page, fill_color);
    draw_rectangle(page, at, width, height);

    RGB stroke_color = {0,0,0};
    set_stroke(page, stroke_color);

    Font font = get_font(project->pdf, "Helvetica-Bold");

    set_fill(page, BLUE);
    set_stroke(page, WHITE);

    HPDF_Page_BeginText(page);
    HPDF_Page_SetFontAndSize(page, font, 44);
    HPDF_Page_SetTextRenderingMode(page, HPDF_FILL_THEN_STROKE);
    HPDF_Page_ShowText(page, feature_name);
    HPDF_Page_EndText(page);
}


static void write_to_file(Project *project) {
    char *filename = strdup(project->name);
    filename = realloc(filename, strlen(filename) + strlen(".pdf") + 1);
    strcat(filename, ".pdf");
    HPDF_SaveToFile(project->pdf, filename);
}


void close_project(Project *project) {
	write_to_file(project);
    free(project);
}
