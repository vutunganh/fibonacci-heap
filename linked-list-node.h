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
struct LinkedListNode*
linkedListNodeInit(struct FhNode* fhNode);

/* Sets `node` as the successor of `result`. Doesn't care about linked lists
 * they belong to, handle that separately!
 */
void
linkedListNodeSetSuccessor(struct LinkedListNode* result,
                           struct LinkedListNode* node);

void
linkedListNodeFree(struct LinkedListNode* llNode);

#endif /* __LINKED_LIST_NODE__ */

