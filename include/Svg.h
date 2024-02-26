#ifndef _LIBRSVGH
#define _LIBRSVGH

#include <librsvg/rsvg.h>

RsvgHandle* new_svg(const char* filename, int is_uri);
void free_svg(RsvgHandle* handle);
// Scales the SVG to the given width and height and renders it to (x, y) on the Cairo context.
int show_svg(cairo_t* cr, RsvgHandle* handle, 
  double x, double y, double width, double height);

#endif
