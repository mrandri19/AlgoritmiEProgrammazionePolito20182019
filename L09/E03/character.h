#ifndef CHARACTER
#define CHARACTER

#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#include "equipment_array.h"
#include "inventory_array.h"
#include "stats.h"

typedef struct {
  int id;
  char name[50 + 1];
  char class[50 + 1];
  int equipment; // Offset in the equipment vector
  stats_t stats;
} character_t;

void character_print(character_t c);

void character_print_with_equipment(character_t c,
                                    equipment_array_t equipments);

character_t character_load_from_keyboard();

// TODO: figure out
// void character_add_object_from_inventory(character_list_t characters,
//                                          equipment_array_t equipments,
//                                          inventory_array_t inventory);

// void character_remove_object_from_inventory(character_list_t characters,
//                                             equipment_array_t equipments,
//                                             inventory_array_t inventory);
#endif