CC := clang
CWARNS := -Wall -pedantic -Wextra
CDEBUG := -DDEBUG -g
LFLAGS := -lm
CFLAGS := -std=c99 -O2 $(CWARNS) $(CDEBUG) $(ARGS)
OBJFILES := linked-list-node.o linked-list.o fh-node.o fibonacci-heap.o
BINFILES := tester.out
STUDENT_ID := 64

build: binfiles

measure: build
	./run-tests.sh

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
	rm *.o

fh-node.o: fh-node.c fh-node.h linked-list.h linked-list-node.h \
 type-declarations.h
fibgen.o: fibgen.c
linked-list.o: linked-list.c linked-list.h linked-list-node.h \
 type-declarations.h fh-node.h
linked-list-node.o: linked-list-node.c linked-list-node.h \
 type-declarations.h
fh-node.o: fh-node.h linked-list.h linked-list-node.h type-declarations.h
fibonacci-heap.o: fibonacci-heap.h linked-list-node.h type-declarations.h \
 fh-node.h linked-list.h
linked-list.o: linked-list.h linked-list-node.h type-declarations.h
linked-list-node.o: linked-list-node.h type-declarations.h
type-declarations.o: type-declarations.h

