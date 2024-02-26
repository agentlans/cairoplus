#include "Svg.h"

RsvgHandle* new_svg(const char* filename, int is_uri) {
  GFile* file;
  RsvgHandle* handle = NULL;
  GError* error = NULL;
  if (is_uri) {
    file = g_file_new_for_uri(filename);
  } else {
    file = g_file_new_for_path(filename);
  }
  handle = rsvg_handle_new_from_gfile_sync(
    file, RSVG_HANDLE_FLAG_KEEP_IMAGE_DATA, NULL, &error);
  if (file) g_object_unref(file);
  if (!handle) {
    g_printerr ("could not open: %s", error->message);
    g_error_free(error);
    return NULL;
  } else {
    return handle;
  }
}

void free_svg(RsvgHandle* handle) {
  g_object_unref(handle);
}

int show_svg(cairo_t* cr, RsvgHandle* handle, 
  double x, double y, double width, double height) {
  RsvgRectangle viewport = {
    .x = x,
    .y = y,
    .width = width,
    .height = height,
  };
  GError* error = NULL;
  if (!rsvg_handle_render_document (handle, cr, &viewport, &error)) {
      g_printerr ("could not render: %s", error->message);
      g_error_free(error);
      return 1;
  } else {
     return 0;
  }
}

