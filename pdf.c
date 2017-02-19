#include "pdf.h"

#include <hpdf.h>
#include <string.h>


/* Some common colours: */
RGB RED = {255,0,0};
RGB GREEN = {0,255,0};
RGB BLUE = {0,0,255};
RGB WHITE = {255,255,255};
RGB BLACK = {0,0,0};


PDF create_pdf(void error_handler()) {
    return HPDF_New(error_handler, NULL);
}

void delete_pdf(PDF pdf) {
    HPDF_Free(pdf);
}

void save_pdf(PDF pdf, const char *basename) {
    char *filename = strdup(basename);
    filename = realloc(filename, strlen(filename) + strlen(".pdf") + 1);
    strcat(filename, ".pdf");
    HPDF_SaveToFile(pdf, filename);
}


Page add_page(PDF pdf) {
    Page page = HPDF_AddPage(pdf);
    HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_LANDSCAPE);
    return page;
}

int get_page_width(Page page) {
    return HPDF_Page_GetWidth(page);
}

int get_page_height(Page page) {
    return HPDF_Page_GetHeight(page);
}


Font get_font(PDF pdf, const char *font_name) {
    return HPDF_GetFont(pdf, font_name, NULL);
}


float get_text_width(Page page, const char *text) {
    return HPDF_Page_TextWidth(page, text);
}


void set_stroke(Page page, RGB rgb) {
    HPDF_Page_SetRGBStroke(page, ((float)rgb.red)/255, ((float)rgb.green)/255, ((float)rgb.blue)/255);
}

void set_fill(Page page, RGB rgb) {
    HPDF_Page_SetRGBFill(page, ((float)rgb.red)/255, ((float)rgb.green)/255, ((float)rgb.blue)/255);
}

void set_dash(Page page, DashPattern pattern) {
    HPDF_Page_SetDash(page, pattern.segment_lengths, pattern.segment_count, 0);
}

void set_font_and_size(Page page, Font font, int font_size) {
    HPDF_Page_SetFontAndSize(page, font, font_size);
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

void write_text(Page page, Position position, TextMode mode, const char *text) {
    HPDF_Page_BeginText(page);
    HPDF_Page_SetTextRenderingMode(page, mode);
    HPDF_Page_MoveTextPos(page, position.x, position.y);
    HPDF_Page_ShowText(page, text);
    HPDF_Page_EndText(page);
}
