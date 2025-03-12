#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "http.h"
#include "pages.h"

Route routes[] = {
  {"GET / ", index_resp},
  {"POST /clicked ", clicked_resp},
  {"GET /user/[0-9]+ ", index_resp},
  {"", fof_resp},
};

void direct(int socketfd) {
  set_socket(socketfd);
  accept_request();
  //writef("test");
  //routes[0].func();
  route(routes, sizeof(routes)/sizeof(*routes));  
}
