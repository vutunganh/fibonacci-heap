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
  struct LinkedList children;
  struct LinkedListNode* llNodePtr;
  struct FhNode* parent;
};

/* Initializes a node of Fibonacci heap.
 */
struct FhNode*
FhNodeInit(int key, int priority);

/* Returns the order of a node of Fibonacci heap.
 */
int
fhNodeOrder(struct FhNode* fhNode);

/* The node with larger priority is added as a child to the other one.
 * Returns the pointer to the single resulting root.
 */
struct FhNode*
FhNodeMerge(struct FhNode* node1, struct FhNode* node2)
__attribute__ ((warn_unused_result));

void
FhNodeFree(struct FhNode* fhNode);

#endif /* __FH_NODE__ */

