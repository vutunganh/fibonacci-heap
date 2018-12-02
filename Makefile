CC := clang
CWARNS := -Wall -pedantic -Wextra
CDEBUG :=#-DDEBUG -g
CFLAGS := -std=c99 -O2 $(CWARNS) $(CDEBUG) $(ARGS)
OBJFILES :=
BINFILES :=
STUDENT_ID := 64

build: binfiles

measure: build
	./run-tests.sh

binfiles: $(BINFILES)

objfiles: $(OBJFILES)

%.o: %.c
	$(CC) $(CFLAGS) $< -c -o $@

%.out: $(OBJFILES)
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean

clean:
	rm *.o

