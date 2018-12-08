#include "linked-list.h"
#include "fh-node.h"

#include <stdlib.h>
#include <assert.h>

struct LinkedList
linkedListInit(void)
{
  struct LinkedList result;
  llHead((&result)) = NULL;
  llTail((&result)) = NULL;
  llSize((&result)) = 0;
  return result;
}

void
linkedListPushBack(struct LinkedList* linkedList, struct FhNode* fhNode)
{
  struct LinkedListNode* llNode = linkedListNodeInit(fhNode);
  if (linkedListIsEmpty(linkedList)) {
    llHead(linkedList) = llNode;
  } else {
    linkedListNodeSetSuccessor(llTail(linkedList), llNode);
  }
  llTail(linkedList) = llNode;
  fhNodeLlNodePtr(fhNode) = llNode;
  ++llSize(linkedList);
}

void
linkedListRemoveNode(struct LinkedList* linkedList,
                     struct LinkedListNode* node)
{
  struct LinkedListNode* prev = llNodePrev(node);
  struct LinkedListNode* next = llNodeNext(node);
  if (NULL == prev && NULL == next) {
    #ifdef DEBUG
    assert(node == llHead(linkedList) && node == llTail(linkedList));
    #endif
    llHead(linkedList) = NULL;
    llTail(linkedList) = NULL;
  } else if (NULL == prev) {
    #ifdef DEBUG
    assert(node == llHead(linkedList));
    #endif
    llNodePrev(next) = NULL;
    llHead(linkedList) = next;
  } else if (NULL == next) {
    #ifdef DEBUG
    assert(node == llTail(linkedList));
    #endif
    llNodeNext(prev) = NULL;
    llTail(linkedList) = prev;
  } else {
    linkedListNodeSetSuccessor(prev, next);
  }
  --llSize(linkedList);
}

bool
linkedListIsEmpty(struct LinkedList* linkedList)
{
  #ifdef DEBUG
  assert(NULL == llHead(linkedList) && NULL == llTail(linkedList));
  assert(NULL == llTail(linkedList) && NULL == llHead(linkedList));
  if (NULL == llHead(linkedList) && NULL == llTail(linkedList)) {
    assert (0 == llSize(linkedList));
  }
  #endif
  return NULL == llHead(linkedList) && NULL == llTail(linkedList);
}

#define clearLinkedList(linkedList) \
  do { \
    llHead(linkedList) = NULL; \
    llTail(linkedList) = NULL; \
    llSize(linkedList) = 0; \
  } while(0)

void
joinLinkedLists(struct LinkedList* result, struct LinkedList* joined)
{
  if (linkedListIsEmpty(result)) {
    *result = *joined;
    return;
  }
  if (linkedListIsEmpty(joined)) {
    return;
  }
  struct LinkedListNode* resultTail = llTail(result);
  struct LinkedListNode* joinedHead = llHead(joined);
  linkedListNodeSetSuccessor(resultTail, joinedHead);
  #ifdef DEBUG
  /* from the second guard it should hold that NULL != tail(joined) */
  assert(NULL != llTail(joined));
  #endif
  llTail(result) = llTail(joined);
  llSize(result) += llSize(joined);
  clearLinkedList(joined);
}

struct LinkedListNode*
linkedListPopFront(struct LinkedList* linkedList)
{
  struct LinkedListNode* result = llHead(linkedList);
  if (NULL == result) {
    return result;
  }

  linkedListRemoveNode(linkedList, result);
  return result;
}

void
linkedListMoveNode(struct LinkedList* from, struct LinkedList* to,
                   struct LinkedListNode* node)
{
  linkedListRemoveNode(from, node);
  struct FhNode* underlyingFhNode = llNodeFhNode(node);
  llNodeFhNode(node) = NULL;
  free(node);
  linkedListPushBack(to, underlyingFhNode);
}

