#include "pdf.h"
#include "vector.h"

typedef struct Project {
    char *name;
    PDF pdf;
    int horizontal_divide;      /* How many columns? */
    int vertical_divide;        /* How many rows? */
    int fractions;              /* How many partials of a row? */
    Vector *feature;
} Project;

typedef struct Feature {
    char *name;
    int length;
    int fractions;
    RGB rgb;
} Feature;

Project *create_project(char *project_name);
void set_horizontal_divide(Project *project, int divide);
void set_vertical_divide(Project *project, int divide);
void set_fractionse(Project *project, int fractions);
void add_board(Project *project, int page_count);
void add_feature(Project *project, char *feature_name, int height_in_fractions, int length);
void close_project(Project *project);
