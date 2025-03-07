CC = clang
CFLAGS =
OBJDIR = obj

.PHONY: build
build: main

.PHONY: run
run: build
	LD_LIBRARY_PATH="./" ./main

$(OBJDIR)/%.o: %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

main: $(OBJDIR)/main.o
	$(CC) $(CLIBS) -o $@ $^


