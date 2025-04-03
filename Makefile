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

$(OBJDIR)/%.o: %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

main: $(OBJDIR)/main.o
	$(CC) $(CLIBS) -o $@ $^


