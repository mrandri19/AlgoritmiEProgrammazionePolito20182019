#ifndef EQUIPMENT_ARRAY
#define EQUIPMENT_ARRAY

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "equipment.h"

typedef struct {
  size_t size;
  size_t capacity;
  equipment_t *equipments;
} equipment_array_t;

equipment_array_t equipment_array_new();

size_t equipment_array_append(equipment_array_t *arr, equipment_t equipment);

size_t
equipment_array_add_equipment_for_character(equipment_array_t *equipments);

equipment_t equipments_array_get_equipment_by_id(equipment_array_t equipments,
                                                 size_t id);

void equipment_array_print(equipment_array_t arr);
void equipment_array_free(equipment_array_t arr);

#endif