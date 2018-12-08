#include "fh-node.h"

#include <stdlib.h>
#include <assert.h>

struct FhNode*
FhNodeInit(int key, int priority)
{
  struct FhNode* result = (struct FhNode*)malloc(sizeof *result);
  fhNodeKey(result) = key;
  fhNodePriority(result) = priority;
  fhNodeMarked(result) = false;
  fhNodeChildren(result) = linkedListInit();
  fhNodeLlNodePtr(result) = NULL;
  fhNodeParent(result) = NULL;
  return result;
}

void
FhNodeAddChild(struct FhNode* root, struct FhNode* child)
{
  linkedListPushBack(&fhNodeChildren(root), child);
  fhNodeParent(child) = root;
}

int
fhNodeOrder(struct FhNode* fhNode)
{
  return linkedListSize(&fhNodeChildren(fhNode));
}

struct FhNode*
FhNodeMerge(struct FhNode* node1, struct FhNode* node2)
{
  #ifdef DEBUG
  assert(NULL != node1 && NULL != node2);
  #endif
  int priority1 = fhNodePriority(node1);
  int priority2 = fhNodePriority(node2);
  if (priority1 < priority2) {
    FhNodeAddChild(node1, node2);
    return node1;
  } else {
    FhNodeAddChild(node2, node1);
    return node2;
  }
}

void
FhNodeFree(struct FhNode* fhNode)
{
  free(fhNode);
}

