#include "fibonacci-heap.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#define fhTrees(fh) (fh->trees)
#define fhSize(fh) (fh->size)
#define fhKeyMap(fh) (fh->keyMap)
#define fhGetKey(fh, key) ((fhKeyMap(fh))[key])

int
max(int a, int b)
{
  return a > b ? a : b;
}

struct FibonacciHeap*
fibonacciHeapInit(int maxKey)
{
  struct FibonacciHeap* res = (struct FibonacciHeap*)malloc(sizeof *res);
  if (NULL == res) {
    fputs("Couldn't allocate Fibonacci heap.", stderr);
    exit(1);
  }
  fhTrees(res) = linkedListInit();
  fhSize(res) = 0;
  fhMaxKey(res) = maxKey;
  fhKeyMap(res) = (struct FhNode**)malloc(maxKey * sizeof(*(fhKeyMap(res))));
  if (NULL == fhKeyMap(res)) {
    fputs("Max key for Fibonacci heap too large.", stderr);
    exit(1);
  }
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
  ++fhSize(fh);
  fhGetKey(fh, key) = fhNode;
}

int
fibonacciHeapFindMaxRank(struct FibonacciHeap* fh)
{
  int res = 0;
  struct LinkedListNode* it = llHead((&fhTrees(fh)));
  while (NULL != it) {
    struct FhNode* curFhNode = llNodeFhNode(it);
    it = llNodeNext(it);
    res = max(res, fhNodeOrder(curFhNode));
  }

  return res;
}

void
fibonacciHeapConsolidate(struct FibonacciHeap* fh)
{
  int maxRank = fibonacciHeapFindMaxRank(fh);
  int buckets = maxRank + linkedListSize(&fhTrees(fh)) + 1;
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
    if (1 == linkedListSize(currentBucket)) {
      linkedListMoveNode(currentBucket, &fhTrees(fh), llHead(currentBucket));
    }
    #ifdef DEBUG
    else {
      assert(0 == linkedListSize(currentBucket));
    }
    #endif
  }

  free(workspace);
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
}

struct FhNode*
fibonacciHeapFindMinNode(struct FibonacciHeap* fh)
{
  int minPriority = (1 << 29);
  struct FhNode* res = NULL;
  struct LinkedListNode* it = llHead((&fhTrees(fh)));
  if (NULL == it) {
    return NULL;
  }

  while (NULL != it) {
    struct FhNode* curFhNode = llNodeFhNode(it);
    it = llNodeNext(it);
    if (fhNodePriority(curFhNode) < minPriority) {
      minPriority = fhNodePriority(curFhNode);
      res = curFhNode;
    }
  }

  return res;
}

bool
fibonacciHeapGetMin(struct FibonacciHeap* fh, int* result)
{
  struct FhNode* res = fibonacciHeapFindMinNode(fh);
  if (NULL == res) {
    return false;
  }
  *result = fhNodePriority(res);
  return true;
}

bool
fibonacciHeapExtractMin(struct FibonacciHeap* fh, int* minKey,
                        int* minPriority)
{
  ++fhExtracts(fh);

  struct FhNode* minNode = fibonacciHeapFindMinNode(fh);
  if (NULL == minNode) {
    return false;
  }
  *minKey = fhNodeKey(minNode);
  *minPriority = fhNodePriority(minNode);
  int oldExtractSteps = fhExtractSteps(fh);

  struct LinkedList* childrenOfMin = &fhNodeChildren(minNode);
  #ifdef DEBUG
  int supposedChildrenCnt = linkedListSize(childrenOfMin);
  int childrenCnt = 0;
  #endif
  fhExtractSteps(fh) += linkedListSize(childrenOfMin);
  struct LinkedListNode* childIterator = llHead(childrenOfMin);
  while (NULL != childIterator) {
    struct FhNode* currentChild = llNodeFhNode(childIterator);
    childIterator = llNodeNext(childIterator);
    fhNodeParent(currentChild) = NULL;
    fhNodeMarked(currentChild) = false;
    #ifdef DEBUG
    ++childrenCnt;
    #endif
  }
  linkedListJoin(&fhTrees(fh), childrenOfMin);
  #ifdef DEBUG
  assert(supposedChildrenCnt == childrenCnt);
  assert(0 == linkedListSize(&fhNodeChildren(minNode)));
  #endif

  struct LinkedListNode* llNodeOfMinNode = fhNodeLlNodePtr(minNode);
  linkedListRemoveNode(&fhTrees(fh), llNodeOfMinNode);
  linkedListNodeFree(llNodeOfMinNode);
  FhNodeFree(minNode);
  fhGetKey(fh, *minKey) = NULL;

  --fhSize(fh);
  fibonacciHeapConsolidate(fh);
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
    if (fhNodeMarked(node)) {
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
fibonacciHeapSize(const struct FibonacciHeap* fh)
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

bool
fibonacciHeapKeyExists(const struct FibonacciHeap* fh, int key)
{
  return NULL != fhGetKey(fh, key);
}

