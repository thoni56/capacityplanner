#include <hpdf.h>

typedef struct Project {
    char *name;
    HPDF_Doc pdf;
    int horisontal_divide;      /* How many columns? */
    int vertical_divide;        /* How many rows? */
    int fractions;                /* How many partials of a row? */
} Project;

Project *create_project(char *project_name, int horisontal_divide, int vertical_divide);
void add_board(Project *project, int count);
void add_feature(Project *project, char *feature_name, int height_in_fractions, int width);
void close_project(Project *project);
