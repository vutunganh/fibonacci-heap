#ifndef __FIBONACCI_HEAP__
#define __FIBONACCI_HEAP__

#include <stdint.h>

#include "linked-list-node.h"
#include "fh-node.h"

#define fhExtracts(fh) (fh->extracts)
#define fhExtractSteps(fh) (fh->extractSteps)
#define fhExtractMax(fh) (fh->extractMax)
#define fhDecreases(fh) (fh->decreases)
#define fhDecreaseSteps(fh) (fh->decreaseSteps)
#define fhDecreaseMax(fh) (fh->decreaseMax)

struct FibonacciHeap
{
  struct LinkedList trees;
  int size;
  struct FhNode* minNode;
  int maxKey;
  int maxRank;
  struct FhNode** keyMap;
  int64_t extracts;
  int64_t extractSteps;
  int64_t extractMax;
  int64_t decreases;
  int64_t decreaseSteps;
  int64_t decreaseMax;
};

struct FibonacciHeap*
fibonacciHeapInit(int maxKey);

void
fibonacciHeapInsert(struct FibonacciHeap* fh, int key, int priority);

/* Extracted key and priority will be returned using parameters `minKey` and
 * `minPriority` respectively.
 * Returns whether the returned values are valid (= the heap wasn't empty)
 * or not.
 */
bool
fibonacciHeapExtractMin(struct FibonacciHeap* fh, int* minKey,
                        int* minPriority);

void
fibonacciHeapDecreaseKey(struct FibonacciHeap* fh, int key, int newPriority);

int
fibonacciHeapSize(struct FibonacciHeap* fh);

/* Current implementation of clear is "slow" and cheating.
 *
 * Slowness dependeds of the "density" of the heap, if the
 * `size` / `maxKey` ratio is high, then it is quite fast, because it doesn't
 * need to traverse the complex structure of the heap.
 * The maximum `maxKey` is around 4 * 10^6 though, so this will only take
 * a few seconds.
 */
void
fibonacciHeapClear(struct FibonacciHeap* fh);

#endif /* __FIBONACCI_HEAP__ */

