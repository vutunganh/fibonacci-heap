CC := clang
CWARNS := -Wall -pedantic -Wextra
CDEBUG :=#-DDEBUG -g -O0
LFLAGS := -lm
CFLAGS := -std=c99 -O2 $(CWARNS) $(CDEBUG) $(ARGS)
OBJFILES := linked-list-node.o linked-list.o fh-node.o fibonacci-heap.o \
  array-heap.o
BINFILES := tester.out judge.out
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

%.out: $(OBJFILES) %.o
	$(CC) $(CFLAGS) $^ -o $@ $(LFLAGS)

tags: *.c *.h
	ctags -R *.c *.h

package:
	mv fibgen.c .fibgen.c
	mkdir vu-tung-anh
	cp *.c *.h Makefile run-tests.sh vu-tung-anh
	tar -czf vu-tung-anh.tgz vu-tung-anh
	rm -r vu-tung-anh
	mv .fibgen.c fibgen.c

.PHONY: clean

clean:
	-rm *.o *.out

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

