#include "http.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <regex.h>

#define BUFSIZE 65536

int SOCKET = 0;
char REQUEST[BUFSIZE+1] = {0};

void writef(char* fmt, ...) {
  va_list args1;
  va_list args2;
  va_start(args1, fmt);
  va_copy(args2, args1);
  size_t s = vsnprintf(NULL, 0, fmt, args1);
  va_end(args1);
  void* str = malloc(s+1);
  if (!str) {
    exit(1);
  }
  s = vsnprintf(str, s+1, fmt, args2);
  va_end(args2);
  write(SOCKET, str, s);
  free(str);
}

void set_socket(int socket) {
  SOCKET = socket;
}

int accept_request() {
  return read(SOCKET, REQUEST, BUFSIZE);
}

char* get_request() {
  return REQUEST;
}

int try_route(Route route) {
  regex_t regex;
  if (regcomp(&regex, route.path, REG_EXTENDED)) {
    exit(1);
  }
  if (!regexec(&regex, get_request(), 0, NULL, 0)) {
    route.func();
    regfree(&regex);
    return 1;
  } else {
    regfree(&regex);
    return 0;
  }
}
void route(Route* routes, int routes_l) {
  for (int i = 0; i < routes_l; ++i) {
    if (try_route(routes[i])) {
      break;
    }
  }
}
