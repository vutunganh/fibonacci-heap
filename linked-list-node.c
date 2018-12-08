#include "linked-list-node.h"

#include <stdlib.h>

struct LinkedListNode*
linkedListNodeInit(struct FhNode* fhNode)
{
  struct LinkedListNode* res = (struct LinkedListNode*)malloc(sizeof *res);
  llNodeFhNode(res) = fhNode;
  llNodePrev(res) = NULL;
  llNodeNext(res) = NULL;
  return res;
}

void
linkedListNodeSetSuccessor(struct LinkedListNode* result,
                           struct LinkedListNode* node)
{
  #ifdef DEBUG
  assert(NULL != result && NULL != node);
  #endif

  llNodeNext(result) = node;
  llNodePrev(node) = result;
}

