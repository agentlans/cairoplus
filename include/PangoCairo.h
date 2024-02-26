#ifndef _PANGOCAIROH
#define _PANGOCAIROH

#include <pango/pangocairo.h>

PangoLayout* new_pango_layout(cairo_t *cr, const char* text, const char* font);
void free_pango_layout(PangoLayout* layout);
// Updates layout with the current Cairo transformation
void update_pango(cairo_t* cr, PangoLayout* layout);
// Width and height in Cairo units
void get_pango_size(PangoLayout* layout, double* width, double* height);
void show_pango(cairo_t* cr, PangoLayout* layout);

#endif
