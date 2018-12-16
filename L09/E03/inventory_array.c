#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inventory.h"
#include "inventory_array.h"

inventory_array_t inventory_array_new() {
  return (inventory_array_t){.size = 0, .capacity = 0, .objects = NULL};
}

// TODO: implement
// void print_equipments_array(equipment_t *equipments, size_t equipments_len) {

//   for (size_t i = 0; i < equipments_len; i++) {
//     printf("[%zu] = ", i);
//     int j = 0;
//     while (j < 8) {
//       object_t *item = equipments[i].equipped[j];
//       if (item == NULL)
//         break;
//       printf("[%p], ", (void *)item);
//       j++;
//     }
//     printf("\n");
//   }
// }

void inventory_array_print(inventory_array_t arr) {
  for (size_t i = 0; i < arr.size; i++) {
    object_print(arr.objects[i]);
  }
}

// object_t *inventory_find_object_by_name(char *name, inventory_t *inventory) {
//   for (size_t i = 0; i < inventory->size; i++) {
//     if (strcmp(inventory->objects[i].name, name) == 0) {
//       return &(inventory->objects[i]);
//     }
//   }
//   return NULL;
// }

// bool insert_into_null_terminated_array(object_t *array[], object_t *o) {
//   int i = 0;
//   while (i < 8) {
//     if (array[i] == NULL) {
//       array[i] = o;
//       return true;
//     } else {
//       i++;
//     }
//   }
//   return false;
// }

// bool remove_from_null_terminated_array(object_t *array[], object_t *o) {
//   int i = 0;
//   int pos = -1;
//   while (array[i] != NULL && i < 8) {
//     if (strcmp(array[i]->name, o->name) == 0) {
//       pos = i;
//       break;
//     }
//   }

//   if (pos == -1) {
//     // object_t not found
//     return false;
//   }
//   array[i] = NULL;

//   // Compact the array (remove the newly created null pointer ) i.e.
//   // {1, 2, NULL, 3, NULL, NULL} -> {1, 2, 3, NULL, NULL}

//   i = 0;
//   while (i < 8 - 1) {
//     if (array[i] == NULL && array[i + 1] != NULL) {
//       array[i] = array[i + 1];
//       array[i + 1] = NULL;
//     }
//     i++;
//   }
//   return true;
// }

inventory_array_t inventory_array_load_file(FILE *fp) {
  int len;
  fscanf(fp, "%d ", &len);

  inventory_array_t arr = {.size = 0, .capacity = 0, .objects = NULL};

  arr.size = arr.capacity = len;
  arr.objects = calloc(arr.size, sizeof(object_t));

  for (int i = 0; i < arr.size; i++) {
    fscanf(fp, "%s %s %d %d %d %d %d %d ", (arr.objects[i].name),
           (arr.objects[i].type), &(arr.objects[i].stats.hp),
           &(arr.objects[i].stats.mp), &(arr.objects[i].stats.atk),
           &(arr.objects[i].stats.def), &(arr.objects[i].stats.mag),
           &(arr.objects[i].stats.spr));
  }

  return arr;
}

void inventory_array_free(inventory_array_t inventory) {
  free(inventory.objects);
}
