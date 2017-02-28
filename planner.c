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
Project *create_project(char *project_name) {
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
    project->horizontal_divide = 2;
    project->vertical_divide = 4;
    project->fractions = 2;

    project->features = create_vector(NULL);
    return project;
}

void set_horizontal_divide(Project *project, int divide) {
    project->horizontal_divide = divide;
}

void set_vertical_divide(Project *project, int divide) {
    project->vertical_divide = divide;
}

void set_fractions(Project *project, int fractions) {
    project->fractions = fractions;
}

static void draw_grid(Project *project, Page page) {
    float length = get_page_width(page)/project->horizontal_divide;
    float height = get_page_height(page)/project->vertical_divide;

    set_stroke(page, BLACK);
    set_fill(page, WHITE);
    for (int row = 0; row < project->vertical_divide; row++)
        for (int column = 0; column < project->horizontal_divide; column++) {
            Position pos = {column*length, row*height};
            draw_rectangle(page, pos, length, height);
        }
}

static void dash_fractions(Project *project, Page page) {
    DashPattern dash_pattern = {2, {3,7}};
    float length = get_page_width(page)/project->horizontal_divide;
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

static void draw_feature_box(Page page, Feature *feature, float width, float height, float offset) {
    Position rectangle_at = {0, offset};
    set_fill(page, feature->rgb);
    draw_rectangle(page, rectangle_at, width, height);
}

static void draw_feature_name(Page page, Feature *feature, Font font, float width, float height, float offset) {
    set_fill(page, BLACK);
    set_stroke(page, WHITE);

    int font_size = 48;
	set_font_and_size(page, font, font_size);
	float text_width = get_text_width(page, feature->name);

    Position text_at = {width/2-text_width/2, height/2-font_size/2+offset};
    write_text(page, text_at, FILL_STROKE, feature->name);
}

static void draw_feature(Project *project, Page page, Feature *feature,
                        int used_fractions) {
    float sprint_width = get_page_width(page)/project->horizontal_divide;
    float fraction_height = get_page_height(page)/project->vertical_divide/project->fractions;
    int length = feature->left_to_draw > project->horizontal_divide?
        project->horizontal_divide:feature->left_to_draw;
    float drawing_width = length*sprint_width;
    float drawing_height = feature->fractions*fraction_height;
    float offset = used_fractions*fraction_height;

	draw_feature_box(page, feature, drawing_width, drawing_height, offset);

    Font font = get_font(project->pdf, "Helvetica-Bold");
	draw_feature_name(page, feature, font, drawing_width, drawing_height,
                      offset);

    feature->left_to_draw -= length;
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
    feature->length = feature->left_to_draw = length_in_sprints;
    feature->rgb = random_colour();

    add_item_to_vector(project->features, feature);
}


static bool feature_fits(int available, Feature *feature, int used) {
    return feature->fractions <= available - used;
}

static void write_to_file(Project *project) {
    Page page = add_page(project->pdf);
    int available_fractions = project->vertical_divide*project->fractions;
    int used_fractions = 0;
    bool features_to_draw = size_of_vector(project->features) > 0;
    while (features_to_draw) {
        features_to_draw = false;
        for (int f = 0; f < size_of_vector(project->features); f++) {
            Feature *feature = (Feature *)get_item_from_vector(project->features, f);
            while (feature->left_to_draw > 0 &&
                   feature_fits(available_fractions, feature, used_fractions)) {
                draw_feature(project, page, feature, used_fractions);
                used_fractions += feature->fractions;
            }
            if (feature->left_to_draw > 0)
                features_to_draw = true;
        }
        if (features_to_draw) {
            page = add_page(project->pdf);
            used_fractions = 0;
        }
    }
	save_pdf(project->pdf, project->name);
}


/*----------------------------------------------------------------------*/
void close_project(Project *project) {
	write_to_file(project);
    delete_pdf(project->pdf);
    free(project);
}
