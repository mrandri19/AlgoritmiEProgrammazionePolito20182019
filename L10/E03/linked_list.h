#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "edge.h"
#include <stdio.h>

typedef struct linked_list {
  edge_t head;
  struct linked_list *tail;
} * linked_list_t;

linked_list_t linked_list_new() { return NULL; }

void linked_list_insert(linked_list_t *list, edge_t edge) {
  if (*list == NULL) {
    (*list) = (linked_list_t)calloc(1, sizeof(struct linked_list));
    (*list)->head = edge;
    (*list)->tail = NULL;
  } else {
    linked_list_insert(&((*list)->tail), edge);
  }
}

void linked_list_fill_vector(linked_list_t list, edge_t *vec, size_t pos) {
  if (list == NULL) {
    return;
  }
  vec[pos] = list->head;

  linked_list_fill_vector(list->tail, vec, pos + 1);
}

size_t linked_list_len(linked_list_t l) {
  if (l == NULL)
    return 0;
  return 1 + linked_list_len(l->tail);
}

void linked_list_free(linked_list_t l) {
  if (l == NULL) {
    return;
  }
  linked_list_free(l->tail);
  free(l);
  return;
}

#endif
