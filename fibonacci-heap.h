#ifndef __FIBONACCI_HEAP__
#define __FIBONACCI_HEAP__

#include "linked-list-node.h"
#include "fh-node.h"

struct FibonacciHeap
{
  struct LinkedList trees;
  struct FhNode* minNode;
  int size;
  struct FhNode* keyMap;
};

struct FibonacciHeap
fibonacciHeapInit(int maxKey);

void
fibonacciHeapInsert(struct FibonacciHeap* fh, int key,  int priority);

/* Doesn't actually return the minimum, because * it isn't needed for this
 * assignment.
 */
void
fibonacciHeapExtractMin(struct FibonacciHeap* fh);

void
fibonacciHeapDecreaseKey(struct FibonacciHeap* fh, int key, int newPriority);

#endif /* __FIBONACCI_HEAP__ */

