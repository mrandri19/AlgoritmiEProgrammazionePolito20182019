#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "equipment.h"

void equipment_print(equipment_t eq) {
  printf("equipment: in_use = %d\n", eq.in_use);
}

int *equipment_get_all_ids(equipment_t *eq) { return eq->inventory_ids; }