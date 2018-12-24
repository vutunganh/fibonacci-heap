#include "fibonacci-heap.h"
#include "array-heap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <unistd.h>
#include <time.h>

#define MAX_TESTS 100
#define MAX_KEY (1<<14)
#define MAX_PRIORITY (1<<14)
#define OP_INSERT 0
#define OP_EXTRACT 1
#define OP_DECREASE 2

FILE* operationsStream = NULL;

bool
keyPresent(const int* const array, const int size, const int key)
{
  for (int i = 0; i < size; ++i) {
    if (key == array[i]) {
      return true;
    }
  }

  return false;
}

int
main(int argc, char* argv[])
{
  srand(time(NULL));
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <operations-file>.\n", argv[0]);
    exit(-1);
  }

  if (access(argv[1], F_OK) != -1) {
    fputs("Operations file already exists, bailing!\n", stderr);
    exit(2);
  } else {
    operationsStream = fopen(argv[1], "w");
  }

  struct FibonacciHeap* fh = fibonacciHeapInit(MAX_KEY);
  struct ArrayHeap* ah = arrayHeapInit(MAX_KEY);

  fprintf(operationsStream, "# %d\n", MAX_KEY);
  for (int _ = 0; _ < MAX_TESTS; ++_) {
    int op = rand() % 3;
    if (OP_INSERT == op) {
      int newKey = 0;
      while (1) {
        newKey = rand() % MAX_KEY;
        const bool ahStatus = arrayHeapKeyExists(ah, newKey);
        const bool fhStatus = fibonacciHeapKeyExists(fh, newKey);
        if (ahStatus != fhStatus) {
          fprintf(stderr, "Mismatch between existence of keys!\n"
                  "AH: %d, FH: %d.\n", ahStatus, fhStatus);
          exit(3);
        }

        if (ahStatus) {
          continue;
        } else {
          break;
        }
      }

      int newPriority = rand() % MAX_PRIORITY;
      fprintf(operationsStream, "I %d %d\n", newKey, newPriority);
      arrayHeapInsert(ah, newKey, newPriority);
      fibonacciHeapInsert(fh, newKey, newPriority);
    } else if (OP_EXTRACT == op) {
      int ahKey = -1, ahPrio = -1;
      bool status;
      fputs("M\n", operationsStream);
      status = arrayHeapExtract(ah, &ahKey, &ahPrio);
      if (!status) {
        assert(0 == fibonacciHeapSize(fh));
      }
      int fhKey = -1, fhPrio = -1;
      fibonacciHeapExtractMin(fh, &fhKey, &fhPrio);
      if (fhPrio != ahPrio) {
        fprintf(stderr, "Mismatch between extracts!\n"
                "FH key: %d, FH prio: %d\n"
                "AH key: %d, AH prio: %d.\n", fhKey, fhPrio, ahKey, ahPrio);
        exit(4);
      }
      if (fhKey != ahKey) {
        ahGetKey(ah, ahKey) = ahPrio;
        ahGetKey(ah, fhKey) = -1;
      }
    } else {
      int key = rand() % MAX_KEY;
      int priority = rand() % MAX_PRIORITY;
      fprintf(operationsStream, "D %d %d\n", key, priority);
      arrayHeapDecreaseKey(ah, key, priority);
      fibonacciHeapDecreaseKey(fh, key, priority);
    }
  }

  arrayHeapClear(ah);
  fibonacciHeapClear(fh);

  return 0;
}
