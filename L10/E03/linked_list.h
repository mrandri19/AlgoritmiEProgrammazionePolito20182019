#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "edge.h"

typedef struct linked_list {
  edge_t head;
  struct linked_list *tail;
} * linked_list_t;

linked_list_t linked_list_new() { return NULL; }

void linked_list_insert(linked_list_t *list, edge_t edge) {
  if (*list == NULL) {
    (*list) = (linked_list_t)calloc(1, sizeof(linked_list_t));
    (*list)->head = edge;
    (*list)->tail = NULL;
  } else {
    linked_list_insert(&((*list)->tail), edge);
  }
}

void linked_list_print(linked_list_t list, size_t index) {
  if (list == NULL) {
    return;
  }

  printf("\t%s <- %d -> %s\n", index, list->head.weight, list->head.id);
  return;
}

#endif
