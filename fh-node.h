#ifndef __FH_NODE__
#define __FH_NODE__

#include "linked-list.h"

#define fhNodeKey(fhNode) (fhNode->key)
#define fhNodePriority(fhNode) (fhNode->priority)
#define fhNodeMarked(fhNode) (fhNode->marked)
#define fhNodeChildren(fhNode) (fhNode->children)
#define fhNodeLlNodePtr(fhNode) (fhNode->llNodePtr)
#define fhNodeParent(fhNode) (fhNode->parent)

struct FhNode
{
  int key;
  int priority;
  bool marked;
  struct LinkedList* children;
  struct LinkedListNode* llNodePtr;
  struct FhNode* parent;
};

/* Initializes a node of Fibonacci heap.
 */
struct FhNode*
FhNodeInit(int key, int priority);

/* Returns the order of a node of Fibonacci heap.
 */
#define fhNodeOrder(fhNode) (llSize(fhNode(children(fhNode))))

#endif /* __FH_NODE__ */

