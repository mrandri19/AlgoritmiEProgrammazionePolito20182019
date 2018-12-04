#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inventory.h"

void print_object(object o) {
  printf("%s %s %d %d %d %d %d %d\n", o.name, o.type, o.stats.hp, o.stats.mp,
         o.stats.atk, o.stats.def, o.stats.mag, o.stats.spr);
}

void print_equipments_array(equipment_t *equipments, size_t equipments_len) {

  for (size_t i = 0; i < equipments_len; i++) {
    printf("[%zu] = ", i);
    int j = 0;
    while (j < 8) {
      object *item = equipments[i].equipped[j];
      if (item == NULL)
        break;
      printf("[%p], ", (void *)item);
      j++;
    }
    printf("\n");
  }
}

void inventory_print(inventory_t *inventory) {
  for (size_t i = 0; i < inventory->size; i++) {
    print_object(inventory->objects[i]);
  }
}

object *inventory_find_object_by_name(char *name, inventory_t *inventory) {
  for (size_t i = 0; i < inventory->size; i++) {
    if (strcmp(inventory->objects[i].name, name) == 0) {
      return &(inventory->objects[i]);
    }
  }
  return NULL;
}

bool insert_into_null_terminated_array(object *array[], object *o) {
  int i = 0;
  while (i < 8) {
    if (array[i] == NULL) {
      array[i] = o;
      return true;
    } else {
      i++;
    }
  }
  return false;
}

bool remove_from_null_terminated_array(object *array[], object *o) {
  int i = 0;
  int pos = -1;
  while (array[i] != NULL && i < 8) {
    if (strcmp(array[i]->name, o->name) == 0) {
      pos = i;
      break;
    }
  }

  if (pos == -1) {
    // object not found
    return false;
  }
  array[i] = NULL;

  // Compact the array (remove the newly created null pointer ) i.e.
  // {1, 2, NULL, 3, NULL, NULL} -> {1, 2, 3, NULL, NULL}

  i = 0;
  while (i < 8 - 1) {
    if (array[i] == NULL && array[i + 1] != NULL) {
      array[i] = array[i + 1];
      array[i + 1] = NULL;
    }
    i++;
  }
  return true;
}

void inventory_load_objects(FILE *fp, inventory_t *inventory) {
  fscanf(fp, "%d ", &(inventory->size));

  inventory->objects = calloc(inventory->size, sizeof(object));
  inventory->capacity = inventory->size;

  for (int i = 0; i < inventory->size; i++) {
    fscanf(
        fp, "%s %s %d %d %d %d %d %d ", (inventory->objects[i].name),
        (inventory->objects[i].type), &(inventory->objects[i].stats.hp),
        &(inventory->objects[i].stats.mp), &(inventory->objects[i].stats.atk),
        &(inventory->objects[i].stats.def), &(inventory->objects[i].stats.mag),
        &(inventory->objects[i].stats.spr));
  }
}

inventory_t inventory_empty() {
  return (inventory_t){.size = 0, .capacity = 0, .objects = NULL};
}