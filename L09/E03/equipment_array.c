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

size_t equipment_array_append(equipment_array_t *arr, equipment_t equipment) {
  printf("called");

  // If empty allocate
  if (arr->capacity == 0) {
    arr->size = 1;
    arr->capacity = 1;
    arr->equipments = calloc(arr->size, sizeof(equipment_t));
  }
  if (arr->size == arr->capacity) {
    arr->size *= 2;
    arr->equipments = realloc(arr->equipments, arr->size);
  }

  arr->equipments[arr->size++] = equipment;

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