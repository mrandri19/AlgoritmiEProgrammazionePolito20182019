#ifndef EQUIPMENT
#define EQUIPMENT

#include "inventory.h"

typedef struct {
  bool in_use;

  // -1 terminated array
  int inventory_ids[8];
} equipment_t;

void equipment_print(equipment_t eq);

bool insert_into_null_terminated_array(object_t *array[], object_t *o);
bool remove_from_null_terminated_array(object_t *array[], object_t *o);

#endif