#ifndef _RASTERH
#define _RASTERH

#include <wand/magick_wand.h>
#include <cairo.h>

typedef unsigned char byte_t;

// Contains an image surface and its underlying data
typedef struct image_surface_t {
	cairo_surface_t* surface; // This is the Cairo object
	byte_t* buffer;
	int stride;
	size_t width, height;
} image_surface_t;

// Creates an image surface with data copied from the MagickWand
image_surface_t* surface_from_wand(MagickWand* mw);
// Creates a new MagickWand using a copy of the data from an image surface
MagickWand* wand_from_surface(image_surface_t* img_surf);

// Deallocates the surface and its underlying data
void free_surface(image_surface_t* img_surf);

// Convenient interface like the Cairo Toy PNG API
image_surface_t* image_surface_create_from_file(const char *filename);
int image_surface_write_to_file(image_surface_t* s, const char* filename);

#endif
