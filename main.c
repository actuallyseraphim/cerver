#include <arpa/inet.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <pthread.h>

enum ReportLevel {
  LOG,
  WARNING,
  ERROR,
};

void __report(const char* file, int line, enum ReportLevel level, const char* fmt, ...) {
  FILE *fptr = fopen("serv.log", "ab+");
  switch (level) {
    case LOG:
      fprintf(fptr, "%s:%i [LOG] ", file, line);
      break;
    case ERROR:
      fprintf(fptr, "%s:%i [ERROR] ", file, line);
      break;
    case WARNING:
      fprintf(fptr, "%s:%i [ERROR] ", file, line);
      break;
  }
  va_list args;
  va_start(args, fmt);
  vfprintf(fptr, fmt, args);
  va_end(args);
  fprintf(fptr, "\n");
  fclose(fptr);

  if (level == ERROR) {
    exit(1);
  }
}
#define report(l, ...) __report(__FILE__, __LINE__, l, __VA_ARGS__)

void (*direct)(int);

//#define RELEASE

int main(void) {
  int port = 8080;
  int err = 0;

  #ifdef RELEASE
  void* lib = dlopen("./content/libdirect.so", RTLD_NOW);
  if (!lib) {
    report(ERROR, "dlopen call %s", dlerror());
  }
  direct = dlsym(lib, "direct");
  if (!direct) {
    report(ERROR, "dlsym call");
  }
  void (*init)(void) = dlsym(lib, "direct");
  if (!init) {
    report(ERROR, "dlsym call");
  }
  #endif

  
  int listenfd = socket(AF_INET, SOCK_STREAM, 0);
  if (listenfd < 0) {
    report(ERROR, "socket call");
  }
  
  int opt = 1;
  err = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

  if (err < 0) {
    report(ERROR, "setsockopt call");
  }

  static struct sockaddr_in serv_addr;

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(port);
  
  bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
  if (err < 0) {
    report(ERROR, "bind call");
  }

  err = listen(listenfd, 64);
  if (err < 0) {
    report(ERROR, "listen call");
  }
  
  static struct sockaddr_in cli_addr;
  socklen_t length = sizeof(cli_addr);
  
  for (;;) {
    int socketfd = accept(listenfd, (struct sockaddr *)&cli_addr, &length);
    if (socketfd < 0) {
      report(ERROR, "accept call");
    }
    
    int pid = fork();
    if (pid < 0) {
      report(ERROR,"fork call");
    } else {
      if(pid == 0) {
	#ifndef RELEASE
	void* lib = dlopen("./content/libdirect.so", RTLD_NOW);
	if (!lib) {
	  report(ERROR, "dlopen call");
	}
	direct = dlsym(lib, "direct");
	if (!direct) {
	  report(ERROR, "dlsym call");
	}
	void (*init)(void) = dlsym(lib, "direct");
	if (!init) {
	  report(ERROR, "dlsym call");
	}
	#endif

	close(listenfd);
	chdir("./content/");
	direct(socketfd);
	#ifndef RELEASE
	dlclose(lib);
	#endif
      } else {
	close(socketfd);
      }
    }
  }
  return 0;
}
