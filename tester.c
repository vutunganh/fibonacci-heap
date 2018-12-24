#include "fibonacci-heap.h"

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

int
main(int argc, char* argv[])
{
  if (2 != argc) {
    fprintf(stderr, "Usage: %s <stats-output-file>.\n", argv[0]);
    exit(1);
  }

  FILE* outputStream = fopen(argv[1], "w");

  struct FibonacciHeap* fh = NULL;
  char op;
  fputs("# heap_size extracts extract_steps extract_max extract_avg "
        "decreases decrease_steps decrease_max decrease_avg\n", outputStream);
  while (1 == scanf(" %c", &op)) {
    int a, b;
    switch(op) {
      case '#':
        if (NULL != fh) {
          fprintf(outputStream, "%d %" PRId64 " %" PRId64 " %" PRId64 " %lf"
                                " %" PRId64 " %" PRId64 " %" PRId64 " %lf\n",
                  fhMaxKey(fh), fhExtracts(fh), fhExtractSteps(fh),
                  fhExtractMax(fh), 1. * fhExtractSteps(fh) / fhExtracts(fh),
                  fhDecreases(fh), fhDecreaseSteps(fh), fhDecreaseMax(fh),
                  1. * fhDecreaseSteps(fh) / fhDecreases(fh));
          fibonacciHeapClear(fh);
        }
        scanf(" %d", &a);
        fh = fibonacciHeapInit(a);
        break;
      case 'I':
        scanf(" %d %d", &a, &b);
        fibonacciHeapInsert(fh, a, b);
        break;
      case 'M':
        fibonacciHeapExtractMin(fh, &a, &b);
        break;
      case 'D':
        scanf(" %d %d", &a, &b);
        fibonacciHeapDecreaseKey(fh, a, b);
        break;
      default:
        fprintf(stderr, "Unknown operation '%c'.\n", op);
        exit(2);
    }
  }

  fibonacciHeapClear(fh);
  fclose(outputStream);

  return 0;
}

