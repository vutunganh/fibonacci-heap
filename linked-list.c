#include "linked-list.h"
#include "fh-node.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define llSize(ll) (ll->size)

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
  fhNodeLlNodePtr(fhNode) = llNode;
  linkedListPushBackLlNode(linkedList, llNode);
}

void
linkedListPushBackLlNode(struct LinkedList* linkedList,
                         struct LinkedListNode* llNode)
{
  if (linkedListIsEmpty(linkedList)) {
    llHead(linkedList) = llNode;
    llNodePrev(llNode) = NULL;
  } else {
    linkedListNodeSetSuccessor(llTail(linkedList), llNode);
  }
  llTail(linkedList) = llNode;
  llNodeNext(llNode) = NULL;
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
  if (NULL == llHead(linkedList) && NULL == llTail(linkedList)) {
    assert (0 == linkedListSize(linkedList));
  }
  #endif
  return NULL == llHead(linkedList) && NULL == llTail(linkedList);
}

int
linkedListSize(struct LinkedList* linkedList)
{
  return llSize(linkedList);
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
  linkedListPushBackLlNode(to, node);
}

void
linkedListJoin(struct LinkedList* result, struct LinkedList* joined)
{
  if (true == linkedListIsEmpty(joined)) {
    return;
  }
  if (true == linkedListIsEmpty(result)) {
    struct LinkedList* tmp = result;
    result = joined;
    joined = tmp;
    return;
  }
  #ifdef DEBUG
  assert(!linkedListIsEmpty(result) && !linkedListIsEmpty(joined));
  #endif

  struct LinkedListNode* resultTail = llTail(result);
  struct LinkedListNode* joinedHead = llHead(joined);
  linkedListNodeSetSuccessor(resultTail, joinedHead);
  llTail(result) = llTail(joined);
  llSize(result) += llSize(joined);
  *joined = linkedListInit();
}

