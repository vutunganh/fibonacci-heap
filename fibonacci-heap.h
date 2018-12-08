#ifndef __FIBONACCI_HEAP__
#define __FIBONACCI_HEAP__

#include "linked-list-node.h"
#include "fh-node.h"

struct FibonacciHeap
{
  struct LinkedList trees;
  struct FhNode* minNode;
  int size;
  struct FhNode** keyMap;
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

void
fibonacciHeapClear(struct FibonacciHeap* fh);

#endif /* __FIBONACCI_HEAP__ */

