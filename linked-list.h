#ifndef __LINKED_LIST__
#define __LINKED_LIST__

#include "linked-list-node.h"

#include <stdbool.h>

#define llHead(ll) (ll->head)
#define llTail(ll) (ll->tail)

struct LinkedList
{
  struct LinkedListNode* head;
  struct LinkedListNode* tail;
  int size;
};

struct LinkedList
linkedListInit(void);

void
linkedListPushBack(struct LinkedList* linkedList, struct FhNode* fhNode);

void
linkedListPushBackLlNode(struct LinkedList* linkedList,
                         struct LinkedListNode* llNode);

/* !!WARNING: doesn't check if `node` belongs to `linkedList`!!
 * !!WARNING: doesn't free `node`!!
 */
void
linkedListRemoveNode(struct LinkedList* linkedList,
                     struct LinkedListNode* node);

/* Returns true if linked list `linkedList` is empty.
 */
bool
linkedListIsEmpty(struct LinkedList* linkedList);

int
linkedListSize(struct LinkedList* linkedList);

/* Returns the head of `linkedList` and removes it from `linkedList`.
 */
struct LinkedListNode*
linkedListPopFront(struct LinkedList* linkedList)
__attribute__ ((warn_unused_result));

/* Moves `node` from `from` to `to`.
 */
void
linkedListMoveNode(struct LinkedList* from, struct LinkedList* to,
                   struct LinkedListNode* node);

/* Appends `joined` to `result`.
 */
void
linkedListJoin(struct LinkedList* result, struct LinkedList* joined);

#endif /* __LINKED_LIST__ */

