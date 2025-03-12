#include "pages.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "http.h"

#define DEFINE_OBJFILE(name) extern char _binary_##name##_start; \
  extern char _binary_##name##_end; \
  int name##_len = &_binary_##name##_end-&_binary_##name##_start; \
  char* name = malloc(name##_len+1); \
  name[name##_len] = 0; \
  memcpy(name, &_binary_##name##_start, name##_len);


void index_resp(void) {
  DEFINE_OBJFILE(index_html);
  writef("HTTP/2 200 OK\n");
  writef("content-type: text/html\n\n");
  writef(index_html);
  free(index_html);
}

void fof_resp(void) {
  DEFINE_OBJFILE(fof_html);
  writef("HTTP/2 200 OK\n");
  writef("content-type: text/html\n\n");
  writef(fof_html);
  free(fof_html);
}

void clicked_resp(void) {
  writef("HTTP/2 200 OK\n");
  writef("content-type: text/html\n\n");
  writef("<h2>Hello, World!</h2>");
}
