#include "Poppler.h"

PopplerDocument* new_pdf_doc(const char* filename, const char* password, int is_uri) {
  GFile* file;
  PopplerDocument* handle = NULL;
  GError* error = NULL;
  if (is_uri) {
    file = g_file_new_for_uri(filename);
  } else {
    file = g_file_new_for_path(filename);
  }
  handle = poppler_document_new_from_gfile(
    file, password, NULL, &error);
  if (file) g_object_unref(file);
  if (!handle) {
    g_printerr ("could not open: %s", error->message);
    g_error_free(error);
    return NULL;
  } else {
    return handle;
  }
}

void free_pdf_doc(PopplerDocument* handle) {
  if (handle) g_object_unref(handle);
}

int get_number_pdf_pages(PopplerDocument* doc) {
  return poppler_document_get_n_pages(doc);
}

PopplerPage* get_pdf_page(PopplerDocument* doc, int index) {
  return poppler_document_get_page(doc, index);
}

void free_pdf_page(PopplerPage* p) {
  if (p) g_object_unref(p);
}

void show_pdf_page(cairo_t* cr, PopplerPage* page) {
  poppler_page_render_for_printing(page, cr);
}

