#include "array-heap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ahHeap(ah) (ah->heap)
#define ahGetKey(ah, key) (ahHeap(ah))[key]
#define ahMaxKey(ah) (ah->maxKey)

int
min(int a, int b)
{
  return a < b ? a : b;
}

struct ArrayHeap*
arrayHeapInit(int maxKey)
{
  struct ArrayHeap* result = NULL;
  result = (struct ArrayHeap*)malloc(sizeof *result);
  if (NULL == result) {
    fputs("Couldn't allocate array heap.\n", stderr);
    exit(1);
  }

  ahHeap(result) = (int*)malloc(maxKey * sizeof(*ahHeap(result)));
  if (NULL == ahHeap(result)) {
    fputs("Max key too large for array heap.\n", stderr);
    exit(1);
  }
  memset(ahHeap(result), -1, maxKey * sizeof(*ahHeap(result)));
  ahMaxKey(result) = maxKey;
  return result;
}

bool
arrayHeapFindMin(struct ArrayHeap* ah, int* const minKey,
                 int* const minPriority)
{
  int resPrio = 1 << 29;
  int resKey = -1;
  for (int i = 0; i < ahMaxKey(ah); ++i) {
    int cur = ahGetKey(ah, i);
    if (-1 < cur && cur < resPrio) {
      resPrio = cur;
      resKey = i;
    }
  }
  if (-1 == resKey) {
    return false;
  }

  *minKey = resKey;
  *minPriority = resPrio;
  return true;
}

int
arrayHeapInsert(struct ArrayHeap* const ah, const int key, const int priority)
{
  if (-1 != ahGetKey(ah, key)) {
    return -1;
  }
  ahGetKey(ah, key) = priority;

  int minK, minP;
  arrayHeapFindMin(ah, &minK, &minP);
  if (priority < minP) {
    return 1;
  } else {
    return 0;
  }
}

bool
arrayHeapExtract(struct ArrayHeap* const ah, int* const minKey,
                 int* const minPriority)
{
  bool res = arrayHeapFindMin(ah, minKey, minPriority);
  if (false == res) {
    return false;
  }

  ahGetKey(ah, *minKey) = -1;
  return true;
}

bool
arrayHeapDecreaseKey(struct ArrayHeap* const ah, int key, int newPriority)
{
  int* value = &(ahGetKey(ah, key));
  if (-1 == *value) {
    return false;
  }

  *value = newPriority;
  return true;
}

void
arrayHeapClear(struct ArrayHeap* ah)
{
  free(ahHeap(ah));
}

bool
arrayHeapKeyExists(struct ArrayHeap* ah, int key)
{
  return -1 != ahGetKey(ah, key);
}

