#include "judge-props.h"
#include "fibonacci-heap.h"
#include "array-heap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>

FILE* operationsStream = NULL;

int
main(void)
{
  srand(time(NULL));
  time_t currentTime = time(NULL);
  struct tm* currentDateTime = localtime(&currentTime);
  const int currentDateTimeStrSize = 128;
  char currentDateTimeStr[currentDateTimeStrSize];
  strftime(currentDateTimeStr, currentDateTimeStrSize,
           "%Y-%m-%d-%H-%M-%S.in", currentDateTime);

  operationsStream = fopen(currentDateTimeStr, "w");

  struct FibonacciHeap* fh = fibonacciHeapInit(MAX_KEY);
  struct ArrayHeap* ah = arrayHeapInit(MAX_KEY);
  int forceDecreases = 0, decreases = 0;

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
      int ahStatus = arrayHeapInsert(ah, newKey, newPriority);
      fibonacciHeapInsert(fh, newKey, newPriority);
      if (1 == ahStatus) {
        int fhMin;
        bool fhMinStatus;
        fhMinStatus = fibonacciHeapGetMin(fh, &fhMin);
        assert(fhMinStatus);
        if (fhMin != newPriority) {
          fputs("Newly added element should've been the minimum!\n", stderr);
        }
      }
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
      ++decreases;
      bool forceDecrease = (rand() % (1 << 5)) != 0;
      int key = rand() % MAX_KEY;
      forceDecrease && ++forceDecreases;
      if (forceDecrease && !arrayHeapEmpty(ah)) {
        for (int i = 0; i < MAX_KEY; ++i) {
          int currentKey = (key + i) % MAX_KEY;
          if (-1 != ahGetKey(ah, currentKey)) {
            key = currentKey;
            break;
          }
        }
      }
      int priority = rand() % MAX_PRIORITY;
      fprintf(operationsStream, "D %d %d\n", key, priority);
      arrayHeapDecreaseKey(ah, key, priority);
      fibonacciHeapDecreaseKey(fh, key, priority);
    }
  }

  arrayHeapClear(ah);
  fibonacciHeapClear(fh);
  fputs("Tests passed.\n", stderr);
  fprintf(stderr, "Decreases: %d, force decreases: %d.\n",
          decreases, forceDecreases);

  return 0;
}

