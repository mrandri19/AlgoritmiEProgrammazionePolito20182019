#include "equipment_array.h"
#include "equipment.h"

equipment_array_t equipment_array_new() {
  printf("created new equipment array\n");
  equipment_array_t arr;
  arr.size = 0;
  arr.capacity = 0;
  arr.equipments = NULL;

  return arr;
}

equipment_t equipments_array_get_equipment_by_id(equipment_array_t equipments,
                                                 size_t id) {
  return equipments.equipments[id];
}

size_t equipment_array_append(equipment_array_t *arr, equipment_t equipment) {
  // If empty allocate
  if (arr->capacity == 0) {
    arr->size = 0;
    arr->capacity = 1;
    arr->equipments = calloc(arr->capacity, sizeof(equipment_t));
  }
  // If full reallocate one double its size
  if (arr->size == arr->capacity) {
    arr->capacity *= 2;
    arr->equipments =
        realloc(arr->equipments, arr->capacity * sizeof(equipment_t));
  }

  arr->equipments[arr->size] = equipment;
  arr->size++;

  return (arr->size - 1);
}

size_t
equipment_array_add_equipment_for_character(equipment_array_t *equipments) {
  equipment_t equipment = {.in_use = false, .inventory_ids = {-1}};
  return equipment_array_append(equipments, equipment);
}

void equipment_array_print(equipment_array_t arr) {
  for (size_t i = 0; i < arr.size; i++) {
    printf("%zu ", i);
    equipment_print(arr.equipments[i]);
  }
}

void equipment_array_free(equipment_array_t arr) { free(arr.equipments); }