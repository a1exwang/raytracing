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
