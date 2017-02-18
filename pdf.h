#ifndef PDF_H
#define PDF_H

#include <hpdf.h>
#include <stdint.h>

typedef HPDF_Doc PDF;
typedef HPDF_Page Page;
typedef HPDF_Font Font;

typedef struct { float x; float y; } Position;
typedef struct { int red; int green; int blue; } RGB;
typedef struct { int part_count; uint16_t lengths[10];} DashPattern;

extern RGB RED;
extern RGB GREEN;
extern RGB BLUE;
extern RGB WHITE;
extern RGB BLACK;

typedef enum {
    FILL = HPDF_FILL,
    STROKE = HPDF_STROKE,
    FILL_STROKE = HPDF_FILL_THEN_STROKE
} TextMode;

extern PDF create_pdf(void error_handler());
extern void delete_pdf(PDF pdf);
extern void save_pdf(PDF pdf, const char *basename);

extern Page add_page(PDF);

extern Font get_font(PDF pdf, const char *font_name);
extern float get_text_width(Page page, const char *text);

extern void set_fill(Page page, RGB rgb);
extern void set_stroke(Page page, RGB rgb);
extern void set_dash(Page page, DashPattern pattern);
extern void set_font_and_size(Page page, Font font, int font_size);

extern void draw_rectangle(Page page, Position at, float width, float height);
extern void draw_line(Page page, Position start, Position end);

#endif
