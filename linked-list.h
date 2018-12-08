#ifndef __LINKED_LIST__
#define __LINKED_LIST__

#include "linked-list-node.h"

#include <stdbool.h>

#define llHead(ll) (ll->head)
#define llTail(ll) (ll->tail)
#define llSize(ll) (ll->size)

struct LinkedList
{
  struct LinkedListNode* head;
  struct LinkedListNode* tail;
  int size;
};

/* Initializes a linked list.
 */
struct LinkedList
linkedListInit(void);

/* Appends a Fibonacci heap node to a linked list.
 */
void
linkedListPushBack(struct LinkedList* linkedList, struct FHNode* fhNode);

/* Removes a concrete node from a linked list.
 */
void
linkedListRemoveNode(struct LinkedList* linkedList,
                     struct LinkedListNode* node);

/* Returns true if linked list `linkedList` is empty.
 */
bool
linkedListIsEmpty(struct LinkedList* linkedList);

/* Appends linked list `linkedList` to `result`.
 */
void
joinLinkedLists(struct LinkedList* result, struct LinkedList* linkedList);

/* Returns the head of `linkedList` and removes it from `linkedList`.
 */
void
linkedListPopFront(struct LinkedList* linkedList);

/* Moves `node` from `from` to `to`.
 */
void
linkedListMoveNode(struct LinkedList* from, struct LinkedList* to,
                   struct LinkedListNode* node);

#endif /* __LINKED_LIST__ */

