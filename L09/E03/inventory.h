#ifndef INVENTORY
#define INVENTORY

#include "stats.h"

typedef struct {
  char name[50 + 1];
  char type[50 + 1];
  stats_t stats;
} object_t;

void object_print(object_t o);

#endif