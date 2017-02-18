#include "pdf.h"
#include "vector.h"

typedef struct Project {
    char *name;
    PDF pdf;
    int horisontal_divide;      /* How many columns? */
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

Project *create_project(char *project_name, int horisontal_divide, int vertical_divide);
void add_board(Project *project, int count);
void add_feature(Project *project, char *feature_name, int height_in_fractions, int width);
void close_project(Project *project);
