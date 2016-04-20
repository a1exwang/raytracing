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



#endif //RAYTRACING_LIST_H
