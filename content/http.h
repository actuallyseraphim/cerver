#pragma once

void writef(char* fmt, ...);
void set_socket(int socket);
int accept_request();
char* get_request();

typedef struct {
  char* path;
  void(*func)(void);
} Route;

int try_route(Route route);
void route(Route* routes, int routes_l);
