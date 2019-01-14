#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_SIZE 1024

typedef struct priority_queue {
  int* items;
  int* priorities;
  size_t size, capacity;
} * priority_queue_t;

priority_queue_t priority_queue_new() {
  priority_queue_t PQ =
      (priority_queue_t)calloc(1, sizeof(struct priority_queue));
  PQ->size = 0;
  PQ->capacity = 0;
  PQ->items = NULL;
  PQ->priorities = NULL;

  return PQ;
}

void priority_queue_free(priority_queue_t PQ) {
  if (PQ->size > 0) {
    free(PQ->items);
    free(PQ->priorities);
  }
  free(PQ);
}

bool priority_queue_size(priority_queue_t PQ) { return PQ->size; }

void priority_queue_add_with_priority(priority_queue_t* PQ, int v,
                                      int priority) {
  // TODO: finish
}
void priority_queue_decrease_priority(priority_queue_t* PQ, int v,
                                      int priority);

int priority_queue_extract_min(priority_queue_t PQ);

#endif
