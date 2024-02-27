#include <stdlib.h>

#include <cairo/cairo-pdf.h>
#include <cairo/cairo-svg.h>

#include "Cairo.h"

int bad_surface(cairo_surface_t* s) {
	return (cairo_surface_status(s) != CAIRO_STATUS_SUCCESS);
}

vector_surface* new_pdf_surface(const char* filename, double width_pt, double height_pt, double dpi) {
	vector_surface* s = malloc(sizeof(vector_surface));
	if (!s) return NULL;
	s->surface = cairo_pdf_surface_create(filename, width_pt, height_pt);
	if (bad_surface(s->surface)) {
		free(s);
		return NULL;
	}
	s->dpi = dpi;
	return s;
}

vector_surface* new_svg_surface(const char* filename, double width_pt, double height_pt, double dpi) {
	vector_surface* s = malloc(sizeof(vector_surface));
	if (!s) return NULL;
	s->surface = cairo_svg_surface_create(filename, width_pt, height_pt);
	if (bad_surface(s->surface)) {
		free(s);
		return NULL;
	}
	s->dpi = dpi;
	return s;
}

void free_surface(vector_surface* s) {
	if (s) {
		cairo_surface_destroy(s->surface);
		free(s);
	}
}

