#include "planner.h"

#include <setjmp.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "pdf.h"
#include "vector.h"

static jmp_buf env;


static void error_handler(HPDF_STATUS error_no, HPDF_STATUS detail_no, void *user_data) {
    printf ("ERROR: error_no=%04X, detail_no=%d\n",
            (unsigned int) error_no, (int) detail_no);
    longjmp (env, 1); /* invoke longjmp() on error */
}


/*----------------------------------------------------------------------*/
Project *create_project(char *project_name, int horisontal_divide, int vertical_divide) {
    PDF pdf;
    Project *project;

    srandom(time(NULL));
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

    project->feature = create_vector(NULL);
    return project;
}


static void draw_grid(Project *project, Page page) {
    float length = get_page_width(page)/project->horisontal_divide;
    float height = get_page_height(page)/project->vertical_divide;

    set_stroke(page, BLACK);
    set_fill(page, WHITE);
    for (int row = 0; row < project->vertical_divide; row++)
        for (int column = 0; column < project->horisontal_divide; column++) {
            Position pos = {column*length, row*height};
            draw_rectangle(page, pos, length, height);
        }
}

static void dash_fractions(Project *project, Page page) {
    DashPattern dash_pattern = {2, {3,7}};
    float length = get_page_width(page)/project->horisontal_divide;
    float height = get_page_height(page)/project->vertical_divide;

    set_dash(page, dash_pattern);
    float fraction_height = height/project->fractions;
    for (int row = 0; row < project->vertical_divide; row++)
        for (int fraction = 1; fraction < project->fractions; fraction++) {
            Position start = {0, row*height+fraction*fraction_height};
            Position end = {get_page_width(page), row*height+fraction*fraction_height};
            draw_line(page, start, end);
        }
}

/*--------------------------------------------------*/
void add_board(Project *project, int page_count) {
    RGB stroke_color = {0,0,0};
    RGB fill_color = {255,255,255};

    for (int i = 0; i < page_count; i++) {
        Page page = add_page(project->pdf);
        draw_grid(project, page);
        dash_fractions(project, page);
    }
}


static int draw_feature(Project *project, Page page, Feature *feature, int used_height) {
    float width = get_page_width(page)/project->horisontal_divide*feature->length;
    float height = get_page_height(page)/project->vertical_divide/project->fractions*feature->fractions;

    Position rectangle_at = {0, used_height};
    set_fill(page, feature->rgb);
    draw_rectangle(page, rectangle_at, width, height);

    Font font = get_font(project->pdf, "Helvetica-Bold");

    set_fill(page, BLACK);
    set_stroke(page, WHITE);

    int font_size = 48;
	set_font_and_size(page, font, font_size);
	float text_width = get_text_width(page, feature->name);

    Position text_at = {width/2-text_width/2, height/2-font_size/2+used_height};
    write_text(page, text_at, FILL_STROKE, feature->name);

    return used_height+height;
}


static RGB random_colour(void){
    RGB rgb;
    rgb.red = (random()>>16)%256;
    rgb.green = (random()>>16)%256;
    rgb.blue = (random()>>16)%256;
    return rgb;
}


/*----------------------------------------------------------------------*/
void add_feature(Project *project, char *feature_name, int height_in_fractions, int length_in_sprints) {
    Feature *feature = (Feature *)malloc(sizeof(Feature));

    feature->name = feature_name;
    feature->fractions = height_in_fractions;
    feature->length = length_in_sprints;
    feature->rgb = random_colour();

    add_item_to_vector(project->feature, feature);
}


static bool feature_fits(Page page, Feature *feature, int used_height) {
    return true;
}

static void write_to_file(Project *project) {
    Page page = add_page(project->pdf);
    int used_height = 0;
    for (int f = 0; f < size_of_vector(project->feature); f++) {
        Feature *feature = (Feature *)get_item_from_vector(project->feature, f);
        if (!feature_fits(page, feature, used_height))
            page = add_page(project->pdf);
        used_height = draw_feature(project, page, feature, used_height);
    }
	save_pdf(project->pdf, project->name);
}


/*----------------------------------------------------------------------*/
void close_project(Project *project) {
	write_to_file(project);
    delete_pdf(project->pdf);
    free(project);
}
