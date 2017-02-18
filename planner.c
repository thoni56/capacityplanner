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
    PDF pdf;
    Project *project;

	pdf = create_pdf(error_handler);
    if (!pdf) {
        printf ("ERROR: cannot create pdf object.\n");
        return NULL;
    }

    if (setjmp(env)) {
        delete_pdf(pdf);
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

float get_text_width(Page page, const char *text) {
    return HPDF_Page_TextWidth(page, text);
}



void add_feature(Project *project, char *feature_name, int height_in_fractions, int length_in_sprints) {
    Page page = add_page(project->pdf);

    float width = PAGE_WIDTH/project->horisontal_divide*length_in_sprints;
    float height = PAGE_HEIGHT/project->vertical_divide/project->fractions*height_in_fractions;

    RGB fill_color = {190, 190, 190};
    Position at = {0,0};
    set_fill(page, fill_color);
    draw_rectangle(page, at, width, height);

    Font font = get_font(project->pdf, "Helvetica-Bold");

    set_fill(page, BLUE);
    set_stroke(page, WHITE);

	set_font_and_size(page, font, 24);
	float w = get_text_width(page, project->name);

    HPDF_TextWidth tw = HPDF_Font_TextWidth(font, project->name, strlen(project->name));
    HPDF_Rect bb = HPDF_Font_GetBBox(font);

    printf("height  = %f\n", bb.top-bb.bottom);

    printf("w       = %f\n", w);
    printf("tw.width= %d\n", tw.width);
    printf("w/tw    = %f\n", w/tw.width);
    printf("tw/w    = %f\n", tw.width/w);

    HPDF_Page_BeginText(page);
    HPDF_Page_SetTextRenderingMode(page, HPDF_FILL_THEN_STROKE);
    HPDF_Page_TextOut(page, width/2-w/2, height/2-12, feature_name);
    HPDF_Page_EndText(page);
}


static void write_to_file(Project *project) {
	save_pdf(project->pdf, project->name);
}


void close_project(Project *project) {
	write_to_file(project);
    free(project);
}
