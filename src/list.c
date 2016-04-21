//
// Created by alexwang on 4/19/16.
//

#include "list.h"

void list_init(ListHead *list) {
  list->prev = list;
  list->next = list;
}

void list_insert(ListHead *list, ListHead *target) {
  target->next = list->next;
  target->prev = list;
  list->next->prev = target;
  list->next = target;
}

void list_insert_before(ListHead *list, ListHead *target) {
  target->next = list;
  target->prev = list->prev;
  list->prev->next = target;
  list->prev = target;
}

void list_node_delete(ListNode *node) {
  node->prev->next = node->next;
  node->next->prev = node->prev;
}

void list_node_insert(ListNode *node, ListNode* target) {
  target->next = node->next;
  target->prev = node;
  node->next->prev = target;
  node->next = target;
}
int list_node_is_empty(const ListNode *node) {
  return node->next == node && node->prev == node;
}

void list_node_init(ListNode *node) {
  node->next = node;
  node->prev = node;
}

