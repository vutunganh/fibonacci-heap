#include "fibonacci-heap.h"

#include <stdlib.h>
#include <assert.h>
#include <math.h>

#define fhTrees(fh) (fh->trees)
#define fhMinNode(fh) (fh->minNode)
#define fhSize(fh) (fh->size)
#define fhKeyMap(fh) (fh->keyMap)
#define fhMaxKey(fh) (fh->maxKey)
#define fhGetKey(fh, key) ((fhKeyMap(fh))[key])
#define fhMaxRank(fh) (fh->maxRank)

int
max(int a, int b)
{
  return a > b ? a : b;
}

struct FibonacciHeap*
fibonacciHeapInit(int maxKey)
{
  struct FibonacciHeap* res = (struct FibonacciHeap*)malloc(sizeof *res);
  fhTrees(res) = linkedListInit();
  fhSize(res) = 0;
  fhMinNode(res) = NULL;
  fhMaxKey(res) = maxKey;
  fhMaxRank(res) = 1;
  fhKeyMap(res) = (struct FhNode**)malloc(maxKey * sizeof(*(fhKeyMap(res))));
  for (int i = 0; i < maxKey; ++i) {
    fhGetKey(res, i) = NULL;
  }
  fhExtracts(res) = 0;
  fhExtractSteps(res) = 0;
  fhExtractMax(res) = 0;
  fhDecreases(res) = 0;
  fhDecreaseSteps(res) = 0;
  fhDecreaseMax(res) = 0;
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
  int buckets = fhMaxRank(fh) + linkedListSize(&fhTrees(fh)) + 1;
  struct LinkedList* workspace =
    (struct LinkedList*)malloc(buckets * sizeof(*workspace));
  for (int i = 0; i < buckets; ++i) {
    workspace[i] = linkedListInit();
  }

  struct LinkedList* rootChildren = &fhTrees(fh);
  struct LinkedListNode* treeIterator = llHead(rootChildren);
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
  for (int i = 0; i < buckets; ++i) {
    struct LinkedList* currentBucket = &workspace[i];
    struct LinkedList* nextBucket = &workspace[i + 1];
    while (linkedListSize(currentBucket) > 1) {
      #ifdef DEBUG
      assert(i < buckets - 1);
      #endif
      struct LinkedListNode* first = linkedListPopFront(currentBucket);
      struct LinkedListNode* second = linkedListPopFront(currentBucket);
      struct FhNode* fhNode1 = llNodeFhNode(first);
      struct FhNode* fhNode2 = llNodeFhNode(second);
      linkedListNodeFree(first);
      linkedListNodeFree(second);
      struct FhNode* mergeResult = FhNodeMerge(fhNode1, fhNode2);
      ++fhExtractSteps(fh);
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
  int minimumPriority = (1 << 30);
  fhMinNode(fh) = NULL;
  fhMaxRank(fh) = 1;
  for (struct LinkedListNode* node = llHead((&fhTrees(fh)));
       NULL != node; node = llNodeNext(node)) {
    struct FhNode* curFhNode = llNodeFhNode(node);
    int currentPriority = fhNodePriority(curFhNode);
    if (currentPriority < minimumPriority) {
      minimumPriority = currentPriority;
      fhMinNode(fh) = llNodeFhNode(node);
    }
    fhMaxRank(fh) = max(fhMaxRank(fh), fhNodeOrder(curFhNode));
  }
}

void
fibonacciHeapMoveToTop(struct FibonacciHeap* fh, struct FhNode* node)
{
  if (NULL == fhNodeParent(node)) {
    return;
  }
  struct LinkedListNode* llNode = fhNodeLlNodePtr(node);
  struct FhNode* parent = fhNodeParent(node);
  struct LinkedList* siblings = &fhNodeChildren(parent);
  linkedListMoveNode(siblings, &fhTrees(fh), llNode);
  fhNodeParent(node) = NULL;
  fhNodeMarked(node) = false;
  fhMaxRank(fh) = max(fhMaxRank(fh), fhNodeOrder(node));
}

bool
fibonacciHeapExtractMin(struct FibonacciHeap* fh, int* minKey,
                        int* minPriority)
{
  ++fhExtracts(fh);

  struct FhNode* minNode = fhMinNode(fh);
  if (NULL == minNode) {
    return false;
  }
  *minKey = fhNodeKey(minNode);
  *minPriority = fhNodePriority(minNode);
  int oldExtractSteps = fhExtractSteps(fh);

  struct LinkedList* childrenOfMin = &fhNodeChildren(minNode);
  fhExtractSteps(fh) += linkedListSize(childrenOfMin);
  struct LinkedListNode* childIterator = llHead(childrenOfMin);
  while (NULL != childIterator) {
    struct FhNode* currentChild = llNodeFhNode(childIterator);
    childIterator = llNodeNext(childIterator);
    fhNodeParent(currentChild) = NULL;
    fhNodeMarked(currentChild) = false;
    fhMaxRank(fh) = max(fhMaxRank(fh), fhNodeOrder(currentChild));
  }
  linkedListJoin(&fhTrees(fh), childrenOfMin);
  #ifdef DEBUG
  assert(0 == linkedListSize(&fhNodeChildren(minNode)));
  #endif

  struct LinkedListNode* llNodeOfMinNode = fhNodeLlNodePtr(minNode);
  linkedListRemoveNode(&fhTrees(fh), llNodeOfMinNode);
  linkedListNodeFree(llNodeOfMinNode);
  FhNodeFree(minNode);
  fhGetKey(fh, *minKey) = NULL;

  --fhSize(fh);
  fibonacciHeapConsolidate(fh);
  fibonacciHeapRecalculateMinimum(fh);

  fhExtractMax(fh) = max(fhExtractMax(fh), fhExtractSteps(fh) - oldExtractSteps);
  return true;
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
      ++fhDecreaseSteps(fh);
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
  if (NULL == node) {
    return;
  }
  if (newPriority >= fhNodePriority(node)) {
    return;
  }

  ++fhDecreases(fh);
  int oldDecreaseSteps = fhDecreaseSteps(fh);

  fhNodePriority(node) = newPriority;
  struct FhNode* parent = fhNodeParent(node);
  if (NULL != parent) {
    ++fhDecreaseSteps(fh);
    fibonacciHeapMoveToTop(fh, node);
    #ifndef NAIVE_FH
    fibonacciHeapCut(fh, parent);
    #endif
  }

  fhDecreaseMax(fh) = max(fhDecreaseMax(fh),
                          fhDecreaseSteps(fh) - oldDecreaseSteps);
}

int
fibonacciHeapSize(struct FibonacciHeap* fh)
{
  return fhSize(fh);
}

void
fibonacciHeapClear(struct FibonacciHeap* fh)
{
  for (int i = 0; i < fhMaxKey(fh); ++i) {
    struct FhNode* curFhNode = fhGetKey(fh, i);
    if (NULL == curFhNode) {
      continue;
    }
    struct LinkedListNode* curLlNode = fhNodeLlNodePtr(curFhNode);
    linkedListNodeFree(curLlNode);
    FhNodeFree(curFhNode);
  }
  free(fhKeyMap(fh));
  fhKeyMap(fh) = NULL;
  free(fh);
}

