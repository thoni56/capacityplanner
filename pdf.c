#include "pdf.h"

#include <hpdf.h>


RGB RED = {255,0,0};
RGB GREEN = {0,255,0};
RGB BLUE = {0,0,255};
RGB WHITE = {255,255,255};
RGB BLACK = {0,0,0};


Page add_page(PDF pdf) {
    Page page = HPDF_AddPage(pdf);
    HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_LANDSCAPE);
    return page;
}


Font get_font(PDF pdf, const char *font_name) {
    return HPDF_GetFont(pdf, font_name, NULL);
}


void set_stroke(Page page, RGB rgb) {
    HPDF_Page_SetRGBStroke(page, ((float)rgb.red)/255, ((float)rgb.green)/255, ((float)rgb.blue)/255);
}

void set_fill(Page page, RGB rgb) {
    HPDF_Page_SetRGBFill(page, ((float)rgb.red)/255, ((float)rgb.green)/255, ((float)rgb.blue)/255);
}

void set_dash(Page page, DashPattern pattern) {
    HPDF_Page_SetDash(page, pattern.lengths, pattern.part_count, 0);
}


void draw_rectangle(Page page, Position position, float width, float height) {
    HPDF_Page_Rectangle(page, position.x, position.y, width, height);
    HPDF_Page_ClosePathFillStroke(page);
}

void draw_line(Page page, Position start, Position end) {
    HPDF_Page_MoveTo(page, start.x, start.y);
    HPDF_Page_LineTo(page, end.x, end.y);
    HPDF_Page_Stroke(page);
}
