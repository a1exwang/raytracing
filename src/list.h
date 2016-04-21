//
// Created by alexwang on 4/19/16.
//

#ifndef RAYTRACING_LIST_H
#define RAYTRACING_LIST_H

typedef struct TListHead {
  struct TListHead *prev, *next;
} ListHead;
void list_init(ListHead *list);
void list_insert(ListHead *list, ListHead *target);
void list_insert_before(ListHead *list, ListHead *target);

#define LIST_ELEMENT(plist, T, list_member) \
  (T*) ((char*)plist - (long long)&((T*)0)->list_member)

#define LIST_FOREACH(plist, T, list_member, var_name) \
  do { \
    ListHead *first = plist; \
    ListHead *list = plist; \
    do  { \
      T *var_name = LIST_ELEMENT(list, T, list_member);

#define LIST_FOREACH_END() \
      list = list->next; \
    } while (list != first); \
  } while (0);

typedef struct TListNode {
  struct TListNode *prev, *next;
  void *data;
} ListNode;

void list_node_init(ListNode *node);
void list_node_insert(ListNode *node, ListNode* target);
void list_node_insert_before(ListNode *node, ListNode *target);
void list_node_delete(ListNode *node);
int list_node_is_empty(const ListNode *node);

#define LIST_NODE_INSERT(list, mydata) \
  do { \
  (list)->data = (mydata); \
  ListNode *new_node = malloc(sizeof(ListNode)); \
  new_node->data = (mydata); \
  list_node_insert((list), new_node);\
  } while (0)

#define LIST_NODE_DATA(list, type) \
  ((type*) list->data)

#define LIST_NODE_REMOVE_LAST(list, type, val) \
  do { \
    ListNode *prev = (list)->prev; \
    list_node_delete(prev); \
    (val) = (type*) prev->data; \
    free(prev); \
  } while (0)

#endif //RAYTRACING_LIST_H
