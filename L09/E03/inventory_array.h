#ifndef INVENTORY_ARRAY
#define INVENTORY_ARRAY

#include "inventory.h"

typedef struct {
  size_t size;
  size_t capacity;
  object_t *objects;
} inventory_array_t;

// Print all of the objects contained into the inventory array
void inventory_array_print(inventory_array_t inventory);

// Create a new inventory array by reading the objects from the file
inventory_array_t inventory_array_load_file(FILE *fp);

// Search by name for an object contained in the inventory array
object_t inventory_array_find_object_by_name(inventory_array_t inventory,
                                             char *name);

// Allocate an empty inventory array
inventory_array_t inventory_array_new();

// Free the inventory array
void inventory_array_free(inventory_array_t inventory);

#endif