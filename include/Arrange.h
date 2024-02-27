#ifndef _ARRANGEH
#define _ARRANGEH
#include <cairo/cairo-pdf.h>
#include <cairo/cairo-svg.h>

// Represents a rectangular patch on some surface
typedef struct patch_t {
	double x;
	double y;
	double width;
	double height;
} patch_t;

// Input: (width,height) of object, (x,y,width,height) of the frame, x and y alignments
// Output: resized object's (x,y,width,height) and scaling factor
void calculate_placement(patch_t* object, double* scale, const patch_t* frame, int x_align, int y_align);

// Calculates the resized image size in pixels to be able to fit in frame at the specified resolution
// Input and output: img_width_px and img_height_px overwritten by resized dimensions
void image_size_px(double* img_width_px, double* img_height_px, double frame_width_pt, double frame_height_pt, double dpi);

#endif
