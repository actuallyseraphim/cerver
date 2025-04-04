CC = clang
CFLAGS =
OBJDIR = obj

SHELL = bash

.PHONY: build
build: libdirect.so

obj:
	mkdir $(OBJDIR)

$(OBJDIR)/%.o: %.c $(wildcard %.h) obj
	$(CC) $(CFLAGS) -c -fPIC -o $@ $<

$(OBJDIR)/%.html.o: html/%.html
	pushd $(dir $<) ; $(LD) -r -b binary $(notdir $<) -o ../$@ ; popd

libdirect.so: $(OBJDIR)/direct.o $(OBJDIR)/pages.o $(OBJDIR)/http.o $(OBJDIR)/index.html.o $(OBJDIR)/fof.html.o
	$(CC) $(CLIBS) -shared -o $@ $^
