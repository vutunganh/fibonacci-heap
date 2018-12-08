#ifndef __LINKED_LIST_NODE__
#define __LINKED_LIST_NODE__

#include "type-declarations.h"

#define llNodeFhNode(llNode) (llNode->fhNode)
#define llNodePrev(llNode) (llNode->prev)
#define llNodeNext(llNode) (llNode->next)

struct LinkedListNode
{
  struct FhNode* fhNode;
  struct LinkedListNode* prev;
  struct LinkedListNode* next;
};

/* Linked list node is only supposed to be a ``wrapper'' for Fibonacci heap
 * node, that's why it's required when initializing.
 */
void
linkedListNodeInit(struct FhNode* fhNode);

/* Sets `node` as the successor of `result`.
 */
void
linkedListNodeSetSuccessor(struct LinkedListNode* result,
                           struct LinkedListNode* node);

#endif /* __LINKED_LIST_NODE__ */

