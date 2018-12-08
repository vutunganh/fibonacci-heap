#include "fibonacci-heap.h"

#include <stdlib.h>
#include <assert.h>
#include <math.h>

#define fhTrees(fh) (fh->trees)
#define fhMinNode(fh) (fh->minNode)
#define fhSize(fh) (fh->size)
#define fhKeyMap(fh) (fh->keyMap)
#define fhGetKey(fh, key) ((fhKeyMap(fh))[key])

struct FibonacciHeap*
fibonacciHeapInit(int maxKey)
{
  struct FibonacciHeap* res = (struct FibonacciHeap*)malloc(sizeof *res);
  fhTrees(res) = linkedListInit();
  fhMinNode(res) = NULL;
  fhSize(res) = 0;
  fhKeyMap(res) = (struct FhNode**)malloc(maxKey * sizeof(*(fhKeyMap(res))));
  return res;
}

void
fibonacciHeapInsert(struct FibonacciHeap* fh, int key, int priority)
{
  struct FhNode* fhNode = FhNodeInit(key, priority);
  linkedListPushBack(&fhTrees(fh), fhNode);
  if (NULL == fhMinNode(fh) ||
      fhNodePriority(fhNode) < fhNodePriority(fhMinNode(fh))) {
    fhMinNode(fh) = fhNode;
  }
  ++fhSize(fh);
  fhGetKey(fh, key) = fhNode;
}

void
fibonacciHeapConsolidate(struct FibonacciHeap* fh)
{
  #ifdef NAIVE_FH
  int buckets = 2 * ceil(sqrt(fibonacciHeapSize(fh)));
  #else
  int buckets = 2 * ceil(log2(fibonacciHeapSize(fh)));
  #endif

  struct LinkedList* workspace =
    (struct LinkedList*)malloc(buckets * sizeof(*workspace));
  struct LinkedList* rootChildren = &fhTrees(fh);

  struct LinkedListNode* treeIterator = llHead((&fhTrees(fh)));
  while (NULL != treeIterator) {
    struct LinkedListNode* tree = treeIterator;
    treeIterator = llNodeNext(treeIterator);

    int order = fhNodeOrder(llNodeFhNode(tree));
    #ifdef DEBUG
    assert(order < buckets);
    #endif
    linkedListMoveNode(rootChildren, workspace + order, tree);
    fhNodeMarked(llNodeFhNode(tree)) = false;
  }
  #ifdef DEBUG
  assert (true == linkedListIsEmpty(rootChildren));
  #endif
  for (int i = 0; i < buckets - 1; ++i) {
    struct LinkedList* currentBucket = &workspace[i];
    struct LinkedList* nextBucket = &workspace[i + 1];
    while (linkedListSize(currentBucket) > 1) {
      struct LinkedListNode* first = linkedListPopFront(currentBucket);
      struct LinkedListNode* second = linkedListPopFront(currentBucket);
      struct FhNode* fhNode1 = llNodeFhNode(first);
      struct FhNode* fhNode2 = llNodeFhNode(second);
      linkedListNodeFree(first);
      linkedListNodeFree(second);
      struct FhNode* mergeResult = FhNodeMerge(fhNode1, fhNode2);
      linkedListPushBack(nextBucket, mergeResult);
    }
    if (linkedListSize(currentBucket) == 1) {
      linkedListMoveNode(currentBucket, &fhTrees(fh), llHead(currentBucket));
    }
  }

  free(workspace);
}

void
fibonacciHeapRecalculateMinimum(struct FibonacciHeap* fh)
{
  int minimumPriority = (1<<30);
  for (struct LinkedListNode* node = llHead((&fhTrees(fh)));
       NULL != node; node = llNodeNext(node)) {
    int currentPriority = fhNodePriority(llNodeFhNode(node));
    if (currentPriority < minimumPriority) {
      minimumPriority = currentPriority;
      fhMinNode(fh) = llNodeFhNode(node);
    }
  }
}

bool
fibonacciHeapExtractMin(struct FibonacciHeap* fh, int* minKey,
                        int* minPriority)
{
  struct FhNode* minNode = fhMinNode(fh);
  if (NULL == minNode) {
    return false;
  }

  /* intentional copy */
  struct LinkedList childrenOfMin = fhNodeChildren(minNode);
  for (struct LinkedListNode* node = llHead((&childrenOfMin)); node != NULL;
       node = llNodeNext(node)) {
    fhNodeMarked(llNodeFhNode(node)) = false;
  }

  linkedListRemoveNode(&fhTrees(fh), fhNodeLlNodePtr(minNode));
  int key = fhNodeKey(minNode);
  *minKey = key;
  *minPriority = fhNodePriority(minNode);
  struct LinkedListNode* llNodeOfMinNode = fhNodeLlNodePtr(minNode);
  /* doesn't clear the children */
  FhNodeFree(minNode);
  linkedListNodeFree(llNodeOfMinNode);
  fhGetKey(fh, key) = NULL;
  joinLinkedLists(&fhTrees(fh), &childrenOfMin);
  --fhSize(fh);
  fibonacciHeapConsolidate(fh);
  fibonacciHeapRecalculateMinimum(fh);

  return true;
}

void
fibonacciHeapMoveToTop(struct FibonacciHeap* fh, struct FhNode* node)
{
  struct LinkedListNode* llNode = fhNodeLlNodePtr(node);
  struct FhNode* parent = fhNodeParent(node);
  struct LinkedList* siblings = fhNodeChildren(parent);
  linkedListMoveNode(siblings, fhTrees(fh), llNode);
  fhNodeParent(node) = NULL;
  fhNodeMarked(node) = false;
}

/* While parents of `node` are marked, keed moving them to top.
 * A bit different from the description in Pruvodce.
 */
void
fibonacciHeapCut(struct FibonacciHeap* fh, struct FhNode* node)
{
  struct FhNode* parent = fhNodeParent(node);
  while (NULL != parent) {
    if (true == fhNodeMarked(node)) {
      fibonacciHeapMoveToTop(fh, node);
      node = parent;
      parent = fhNodeParent(node);
    } else {
      fhNodeMarked(node) = true;
      break;
    }
  }
}

void
fibonacciHeapDecreaseKey(struct FibonacciHeap* fh, int key, int newPriority)
{
  struct FhNode* node = fhGetKey(fh, key);
  if (newPriority >= fhNodePriority(node)) {
    return;
  }
  fhNodePriority(node) = newPriority;
  struct FhNode* parent = fhNodeParent(node);
  fibonacciHeapMoveToTop(fh, node);
  fibonacciHeapCut(fh, parent);
}

int
fibonacciHeapSize(struct FibonacciHeap* fh)
{
  return fhSize(fh);
}

void
fibonacciHeapClear(struct FibonacciHeap* fh)
{
}

