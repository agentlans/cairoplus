#include "PangoCairo.h"

PangoLayout* new_pango_layout(cairo_t *cr, const char* text, const char* font) {
  PangoLayout *layout;
  PangoFontDescription *desc;

  // Create a PangoLayout, set the font and text
  layout = pango_cairo_create_layout(cr);
  pango_layout_set_text(layout, text, -1);

  desc = pango_font_description_from_string(font);
  pango_layout_set_font_description(layout, desc);
  pango_font_description_free(desc);
  
  return layout;
}

void free_pango_layout(PangoLayout* layout) {
  g_object_unref(layout);
}

// Updates layout with the current Cairo transformation
void update_pango(cairo_t* cr, PangoLayout* layout) {
  // Inform Pango to re-layout the text with the new transformation
  pango_cairo_update_layout(cr, layout);
}

void get_pango_size(PangoLayout* layout, double* width, double* height) {
  int pango_width, pango_height;
  // Get the size of the text
  pango_layout_get_size(layout, &pango_width, &pango_height);
  *width = ((double) pango_width) / PANGO_SCALE;
  *height = ((double) pango_height) / PANGO_SCALE;
}

void show_pango(cairo_t* cr, PangoLayout* layout) {
  pango_cairo_show_layout(cr, layout);
}

