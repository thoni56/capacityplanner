#include <hpdf.h>

typedef struct {
    char *name;
    HPDF_Doc pdf;
} Project;

Project *create_project(char *project_name);
void add_board(Project *project);
void close_project(Project *project);
