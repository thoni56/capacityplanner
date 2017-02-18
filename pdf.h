#ifndef PDF_H
#define PDF_H

#include <hpdf.h>
#include <stdint.h>

typedef HPDF_Doc PDF;
typedef HPDF_Page Page;

typedef struct { float x; float y; } Position;
typedef struct { int red; int green; int blue; } RGB;
typedef struct { int part_count; uint16_t lengths[10];} DashPattern;

extern RGB RED;
extern RGB GREEN;
extern RGB BLUE;
extern RGB WHITE;
extern RGB BLACK;


extern Page add_page(PDF);
extern void set_fill(Page page, RGB rgb);
extern void set_stroke(Page page, RGB rgb);
extern void set_dash(Page page, DashPattern pattern);
extern void draw_rectangle(Page page, Position at, float width, float height);
extern void draw_line(Page page, Position start, Position end);

#endif
