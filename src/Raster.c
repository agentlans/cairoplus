#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#include "Raster.h"

// Returns the byte at index i of x.
// Most significant byte is index 0.
byte_t get_byte(uint32_t x, int i) {
  return (x & (0xFF << (8 * (3-i)))) >> (8 * (3-i));
}

// Sets the byte at index i of *x.
// Most significant byte is index 0.
void set_byte(uint32_t* x, int i, byte_t y) {
  *x = (*x) & ~(0xFF << (8 * (3-i))); // Set the byte to 0x00.
  *x = (*x) | (y << (8 * (3 - i))); // Set the byte to y
}

// double to byte
static inline byte_t dtoc(double x) {
  return 0xFF * x;
}

// byte to double
static inline double ctod(byte_t x) {
  return ((double) x) / 0xFF;
}

void write_argb32(uint32_t* out, double alpha, double red, double green, double blue) {
  set_byte(out, 0, dtoc(alpha));
  set_byte(out, 1, dtoc(red * alpha));
  set_byte(out, 2, dtoc(green * alpha));
  set_byte(out, 3, dtoc(blue * alpha));
}

// Reads the ARGB32 format colour into an array of doubles
void read_argb32(double comps[4], const byte_t* pixel) {
  uint32_t px = ((uint32_t*) pixel)[0];
  double alpha = comps[0] = ctod(get_byte(px, 0));
  if (alpha == 0) {
    // Completely transparent
    memset(comps, 0, 4);
    return;
  }
  comps[1] = ctod(get_byte(px, 1) / alpha);
  comps[2] = ctod(get_byte(px, 2) / alpha);
  comps[3] = ctod(get_byte(px, 3) / alpha);
}

// Reads the image file into a MagickWand and returns the Wand.
MagickWand* new_wand(const char* filename, unsigned long *width, unsigned long *height) {
  MagickWand* mw = NULL;
  mw = NewMagickWand();
  if (!mw) return NULL;
  MagickReadImage(mw, filename);
  if (width) *width = MagickGetImageWidth(mw);
  if (height) *height = MagickGetImageHeight(mw);
  return mw;
}

// Returns a Wand using pixel information from the buffer
MagickWand* new_wand_from_buffer(byte_t* buffer, unsigned long width, unsigned long height, size_t stride) {
  MagickWand* mw = NULL;
  mw = NewMagickWand();
  if (!mw) return NULL;
  // Create a new image
  MagickSetSize(mw, width, height);
  MagickReadImage(mw, "xc:none");
  // Iterate over pixels
  PixelIterator* imw;
  PixelWand** pmw;
  imw = NewPixelIterator(mw);
  unsigned long y;
  register long x;
  
  byte_t* p = buffer; // Current position in the buffer
  byte_t* row_start = buffer;
  double colour[4]; // Current pixel colour
  for (y = 0; y < height; ++y) {
    pmw = PixelGetNextIteratorRow(imw, &width);
    p = row_start;
    for (x = 0; x < (long) width; ++x) {
      read_argb32(colour, p);
      PixelSetAlpha(pmw[x], colour[0]);
      PixelSetRed(pmw[x], colour[1]);
      PixelSetGreen(pmw[x], colour[2]);
      PixelSetBlue(pmw[x], colour[3]);
      p += 4;
    }
    row_start += stride;
    PixelSyncIterator(imw);
  }
  imw = DestroyPixelIterator(imw);
  return mw;
}

void free_wand(MagickWand* mw) {
  DestroyMagickWand(mw);
}

// Reads image data from wand into a buffer.
// Note: buffer must be set to NULL before calling!
void read_from_wand(byte_t** buffer, MagickWand* mw, size_t stride) {
  unsigned long width,height;
  width = MagickGetImageWidth(mw);
  height = MagickGetImageHeight(mw);
  // Allocate buffer to store pixel data
  *buffer = realloc(*buffer, stride * height * 4);
  byte_t* p = *buffer;
  // Iterate over pixels
  PixelIterator* imw;
  PixelWand** pmw;
  imw = NewPixelIterator(mw);
  unsigned long y;
  register long x;
  byte_t* row_start = *buffer;
  for (y = 0; y < height; ++y) {
    pmw = PixelGetNextIteratorRow(imw, &width);
    p = row_start;
    for (x = 0; x < (long) width; ++x) {
      write_argb32(((uint32_t*) p),
                   PixelGetAlpha(pmw[x]),
                   PixelGetRed(pmw[x]),
                   PixelGetGreen(pmw[x]),
                   PixelGetBlue(pmw[x]));
      p += 4;
    }
    row_start += stride;
  }
  imw = DestroyPixelIterator(imw);
}

// Creates an image surface with data copied from the MagickWand
image_surface_t* surface_from_wand(MagickWand* mw) {
  // Initialize the image_surface_t* on the heap
  image_surface_t* img_surf;
  img_surf = malloc(sizeof(image_surface_t));
  if (!img_surf) return NULL;
  *img_surf = (image_surface_t) {NULL, NULL, 0, 0, 0};
  // Set properties of the object
  img_surf->width = MagickGetImageWidth(mw);
  img_surf->height = MagickGetImageHeight(mw);
  img_surf->stride = cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, img_surf->width);
  read_from_wand(&(img_surf->buffer), mw, img_surf->stride);
  // Set the data
  img_surf->surface = cairo_image_surface_create_for_data(
  	img_surf->buffer, CAIRO_FORMAT_ARGB32, 
  	img_surf->width, img_surf->height, img_surf->stride);
  return img_surf;
}

// Deallocates the surface and its underlying data
void free_image_surface(image_surface_t* img_surf) {
  if (img_surf) {
    cairo_surface_destroy(img_surf->surface);
    free(img_surf->buffer);
    free(img_surf);
  }
}

// Creates a new MagickWand using a copy of the data from an image surface
MagickWand* wand_from_surface(image_surface_t* img_surf) {
  MagickWand* mw = new_wand_from_buffer(
    img_surf->buffer,
    img_surf->width,
    img_surf->height,
    img_surf->stride);
  return mw;
}

image_surface_t* image_surface_create_from_file(const char *filename) {
  MagickWand* m_wand = NewMagickWand();
  if (!m_wand) return NULL;
  if (MagickReadImage(m_wand, filename) == MagickFalse) {
    // Couldn't read the file
    return NULL;
  }
  // Make a new surface
  image_surface_t* out = NULL;
  out = surface_from_wand(m_wand);
  // Clean up
  DestroyMagickWand(m_wand);
  return out;
}

int image_surface_write_to_file(image_surface_t* s, const char* filename) {
  MagickWand* mw = wand_from_surface(s);
  if (!mw) return 1;
  if (MagickWriteImage(mw, filename) == MagickFalse) {
    DestroyMagickWand(mw);
    return 1;
  }
  if (mw) DestroyMagickWand(mw);
  return 0;
}

