#ifndef _CAIROH
#define _CAIROH

#include <cairo/cairo.h>

typedef struct vector_surface {
	cairo_surface_t* surface;
	double dpi;
} vector_surface;

vector_surface* new_pdf_surface(const char* filename, double width_pt, double height_pt, double dpi);
vector_surface* new_svg_surface(const char* filename, double width_pt, double height_pt, double dpi);
void free_surface(vector_surface* s);

#endif
