CC := clang
CWARNS := -Wall -pedantic -Wextra
CDEBUG := -DDEBUG -g -O0
LFLAGS := -lm
CFLAGS := -std=c99 -O2 $(CWARNS) $(CDEBUG) $(ARGS)
OBJFILES := linked-list-node.o linked-list.o fh-node.o fibonacci-heap.o tester.o
BINFILES := tester.out
STUDENT_ID := 64

build: binfiles

measure: build
	./run-tests.sh

generator: fibgen.c
	$(CC) $(CFLAGS) $< -o $@ $(LFLAGS)

binfiles: $(BINFILES)

objfiles: $(OBJFILES)

%.o: %.c
	$(CC) $(CFLAGS) $< -c -o $@

%.out: $(OBJFILES)
	$(CC) $(CFLAGS) $^ -o $@ $(LFLAGS)

tags: *.c *.h
	ctags -R *.c *.h

.PHONY: clean

clean:
	-rm *.o *.out vgcore.*

fh-node.o: fh-node.c fh-node.h linked-list.h linked-list-node.h \
 type-declarations.h
fibonacci-heap.o: fibonacci-heap.c fibonacci-heap.h linked-list-node.h \
 type-declarations.h fh-node.h linked-list.h
linked-list.o: linked-list.c linked-list.h linked-list-node.h \
 type-declarations.h fh-node.h
linked-list-node.o: linked-list-node.c linked-list-node.h \
 type-declarations.h
tester.o: tester.c fibonacci-heap.h linked-list-node.h \
 type-declarations.h fh-node.h linked-list.h

