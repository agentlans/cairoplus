#include "Arrange.h"

double arrange_offset(double object_size, double frame_size, int align) {
	if (align < 0) { // left or top align
		return 0;
	} else if (align == 0) { // horizontal or vertical centre
		double offset = (frame_size - object_size) / 2;
		return offset;
	} else { // right or bottom align
		return frame_size - object_size;
	}
}

// Return the max magnification of a width x height object to fit in frame_width x frame_height space
double magnification(double width, double height, double frame_width, double frame_height) {
	double x_ratio = frame_width / width;
	double y_ratio = frame_height / height;
	if (x_ratio < y_ratio) {
		return x_ratio;
	} else {
		return y_ratio;
	}
}

// Input: (width,height) of object, (x,y,width,height) of the frame, x and y alignments
// Output: resized object's (x,y,width,height) and scaling factor
void calculate_placement(patch_t* object, double* scale, const patch_t* frame, int x_align, int y_align) {
	double mag = magnification(object->width, object->height, frame->width, frame->height);
	*scale = mag;
	double new_w = object->width * mag;
	double new_h = object->height * mag;
	double x_offset = arrange_offset(new_w, frame->width, x_align);
	double y_offset = arrange_offset(new_h, frame->height, y_align);
	object->x = frame->x + x_offset;
	object->y = frame->y + y_offset;
	object->width = new_w;
	object->height = new_h;
}

const int POINTS_PER_INCH = 72;

// Converts number of points to number of pixels at the specified resolution
double pt_to_px(double pt, double dpi) {
	return (pt / POINTS_PER_INCH) * dpi;
}

// Calculates the resized image size in pixels to be able to fit in frame at the specified resolution
// Input and output: img_width_px and img_height_px overwritten by resized dimensions
void image_size_px(double* img_width_px, double* img_height_px, double frame_width_pt, double frame_height_pt, double dpi) {
	// Convert frame dimensions to pixels
	double frame_width_px = pt_to_px(frame_width_pt, dpi);
	double frame_height_px = pt_to_px(frame_height_pt, dpi);
	// Find the magnification for the image to fit in the frame
	double m = magnification(*img_width_px, *img_height_px, frame_width_px, frame_height_px);
	// Update the image's pixel dimensions
	*img_width_px = (*img_width_px) * m;
	*img_height_px = (*img_height_px) * m;
}

