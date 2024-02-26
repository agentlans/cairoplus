#ifndef _POPPLERH
#define _POPPLERH

#include <poppler.h>

PopplerDocument* new_pdf_doc(const char* filename, const char* password, int is_uri);
void free_pdf_doc(PopplerDocument* handle);
int get_number_pdf_pages(PopplerDocument* doc);

PopplerPage* get_pdf_page(PopplerDocument* doc, int index);
void free_pdf_page(PopplerPage* p);
void show_pdf_page(cairo_t* cr, PopplerPage* page);

#endif
