#ifndef __FIBONACCI_HEAP__
#define __FIBONACCI_HEAP__

#include <stdint.h>
#include <stdbool.h>

#include "linked-list-node.h"
#include "fh-node.h"

#define fhGetMin(fh) (fhNodePriority(fhMinNode(fh)))
#define fhMaxKey(fh) (fh->maxKey)
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

bool
fibonacciHeapGetMin(struct FibonacciHeap* fh, int* result);

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
fibonacciHeapSize(const struct FibonacciHeap* fh);

/* Current implementation of clear is "slow" and cheating.
 *
 * Slowness dependeds of the "density" of the heap, if the
 * `size` / `maxKey` ratio is high, then it is quite fast, because it doesn't
 * need to traverse the complex structure of the heap.
 * The maximum `maxKey` is around 10^7 though, so this will only take
 * a few seconds at most and is still comparable to traversing the heap.
 */
void
fibonacciHeapClear(struct FibonacciHeap* fh);

/* Detects if key `key` exists. Used for testing.
 */
bool
fibonacciHeapKeyExists(const struct FibonacciHeap* fh, int key);
#endif /* __FIBONACCI_HEAP__ */

