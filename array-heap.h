#ifndef __ARRAY_HEAP__
#define __ARRAY_HEAP__

#include <stdbool.h>

#define ahHeap(ah) (ah->heap)
#define ahGetKey(ah, key) (ahHeap(ah))[key]
#define ahMaxKey(ah) (ah->maxKey)

struct ArrayHeap
{
  int* heap;
  int maxKey;
};

struct ArrayHeap*
arrayHeapInit(int maxKey);

/* Returns -1 if the key has already existed.
 * Returns 0 if the key hasn't existed and the newly inserted key **isn't** the
 * minimum.
 * Returns 1 if the key hasn't existed and the newly inserted key **is** the
 * minimum.
 */
int
arrayHeapInsert(struct ArrayHeap* ah, int key, int priority);

/* Returns false if the heap was empty.
 */
bool
arrayHeapExtract(struct ArrayHeap* const ah, int* const minKey,
                 int* const minPriority);

/* Returns false if the key didn't exist.
 */
bool
arrayHeapDecreaseKey(struct ArrayHeap* const ah, int key, int newPriority);

void
arrayHeapClear(struct ArrayHeap* ah);

bool
arrayHeapKeyExists(struct ArrayHeap* ah, int key);

bool
arrayHeapEmpty(const struct ArrayHeap* const ah);

#endif /* __ARRAY_HEAP__ */

