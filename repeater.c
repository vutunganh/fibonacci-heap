#include "judge-props.h"
#include "array-heap.h"
#include "fh-node.h"
#include "fibonacci-heap.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

FILE* opStream = NULL;

void
consistencyCheck(const struct FibonacciHeap* const fh,
                 const struct ArrayHeap* const ah,
                 int maxKey, int opNum)
{
  for (int i = 0; i < maxKey; ++i) {
    struct FhNode* curFhNode = fhGetKey(fh, i);
    const int curAhVal = ahGetKey(ah, i);
    if (-1 == curAhVal) {
      if (NULL != curFhNode) {
        fprintf(stderr, "Error on line %d.\n"
                "Key %d isn't supposed to be in the heap!\n", opNum, curAhVal);
        exit(5);
      } else {
        continue;
      }
    }
    const int curFhVal = fhNodePriority(curFhNode);
    if (curFhVal != curAhVal) {
      fprintf(stderr, "Error on line %d.\n"
              "Mismatch between keys %d!\n"
              "FH: %d, AH: %d\n",
              opNum, i, fhNodePriority(curFhNode), curAhVal);
      exit(5);
    }
  }
}

int
main(int argc, char* argv[])
{
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <operations-file>.\n", argv[0]);
    exit(-1);
  }
  opStream = fopen(argv[1], "r");

  struct FibonacciHeap* fh = NULL;
  struct ArrayHeap* ah = NULL;
  char op;
  int bkMaxKey;
  int opNumber = 1;
  while (1 == fscanf(opStream, " %c", &op)) {
    if (op == '#') {
      int maxKey;
      fscanf(opStream, " %d", &maxKey);
      bkMaxKey = maxKey;
      fh = fibonacciHeapInit(maxKey);
      ah = arrayHeapInit(maxKey);
    } else if (op == 'I') {
      int key, priority;
      fscanf(opStream, " %d %d", &key, &priority);
      consistencyCheck(fh, ah, bkMaxKey, opNumber);
      int ahStatus = arrayHeapInsert(ah, key, priority);
      fibonacciHeapInsert(fh, key, priority);
      if (1 == ahStatus) {
        int fhMin;
        bool fhMinStatus;
        fhMinStatus = fibonacciHeapGetMin(fh, &fhMin);
        assert(fhMinStatus);
        if (fhMin != priority) {
          fputs("Newly added element should've been the minimum!\n", stderr);
        }
      }
      consistencyCheck(fh, ah, bkMaxKey, opNumber);
    } else if (op == 'M') {
      int ahKey = -1, ahPrio = -1;
      bool status;
      consistencyCheck(fh, ah, bkMaxKey, opNumber);
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
      consistencyCheck(fh, ah, bkMaxKey, opNumber);
    } else if (op == 'D') {
      int key, priority;
      fscanf(opStream, " %d %d", &key, &priority);
      consistencyCheck(fh, ah, bkMaxKey, opNumber);
      arrayHeapDecreaseKey(ah, key, priority);
      fibonacciHeapDecreaseKey(fh, key, priority);
      consistencyCheck(fh, ah, bkMaxKey, opNumber);
    }
    ++opNumber;
  }

  arrayHeapClear(ah);
  fibonacciHeapClear(fh);
  fclose(opStream);
  return 0;
}
