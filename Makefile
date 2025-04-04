CC = clang
CFLAGS =
OBJDIR = obj

SHELL = bash

.PHONY: build
build: main
	make -C ./content/

.PHONY: run
run: build
	LD_LIBRARY_PATH="./" ./main

obj:
	mkdir $(OBJDIR)

$(OBJDIR)/%.o: %.c $(wildcard %.h) obj
	$(CC) $(CFLAGS) -c -o $@ $<

main: $(OBJDIR)/main.o
	$(CC) $(CLIBS) -o $@ $^


