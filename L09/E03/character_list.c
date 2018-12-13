#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "character.h"
#include "character_list.h"
#include "equipment_array.h"

linked_list_t *linked_list_new() { return NULL; }

linked_list_t *linked_list_insert_character(linked_list_t *list,
                                            character_t c) {
  if (list == NULL) {
    linked_list_t *l = calloc(1, sizeof(linked_list_t));
    l->head = c;
    l->tail = NULL;
    return l;
  } else {
    list->tail = linked_list_insert_character(list->tail, c);
    return list;
  }
}

linked_list_t *remove_character_id_linked_list(linked_list_t *list, int id) {
  if (list == NULL) {
    return NULL;
  } else {
    if (list->head.id == id) {
      return list->tail;
    } else {
      list->tail = remove_character_id_linked_list(list->tail, id);
      return list;
    }
  }
}

void linked_list_print_character(linked_list_t *list) {
  if (list == NULL)
    return;
  character_t c = list->head;

  character_print(c);
  linked_list_print_character(list->tail);
}

void linked_list_free(linked_list_t *list) {
  if (list == NULL) {
    return;
  } else {
    linked_list_free(list->tail);
    free(list);
  }
}

character_t *find_character_in_linked_list(linked_list_t *list, int id) {
  if (list == NULL) {
    return NULL;
  } else {
    if (list->head.id == id) {
      return &(list->head);
    } else {
      return find_character_in_linked_list(list->tail, id);
    }
  }
}

character_list_t character_list_new() {
  return (character_list_t){.first = linked_list_new(),
                            .last = linked_list_new()};
}

character_t character_list_find_character_by_id(character_list_t list, int id) {
  return *find_character_in_linked_list(list.first, id);
}

character_list_t character_list_remove_character_by_id(character_list_t list,
                                                       int id) {
  list.first = remove_character_id_linked_list(list.first, id);
  return list;
}

void character_list_print(character_list_t list) {
  linked_list_print_character(list.first);
}

void character_list_free(character_list_t list) {
  linked_list_free(list.first);
}

// character_list_t last_element_linked_list(character_list_t list) {
//   if (list == NULL) {
//     exit(1);
//   }
//   if (list->tail == NULL) {
//     return list;
//   } else {
//     return last_element_linked_list(list->tail);
//   }
// }

character_list_t character_list_add_character(character_list_t list,
                                              character_t c) {
  list.first = linked_list_insert_character(list.first, c);
  return list;
}

character_list_t character_list_from_file(FILE *fp,
                                          equipment_array_t *equipments) {

  printf("before");
  character_list_t characters = character_list_new();
  printf("asdawdaw");

  int len = 0;
  fscanf(fp, "%d ", &len);

  // then for each character
  for (int i = 0; i < len; i++) {
    printf("loading");

    character_t c;
    fscanf(fp, "PG%d %s %s %d %d %d %d %d %d ", &c.id, c.name, c.class,
           &c.stats.hp, &c.stats.mp, &c.stats.atk, &c.stats.def, &c.stats.mag,
           &c.stats.spr);
    c.equipment = equipment_array_add_equipment_for_character(equipments);

    characters = character_list_add_character(characters, c);
  }

  return characters;
}

// void load_characters_from_keyboard(character_list_t *characters,
//                                    equipment_t **equipments,
//                                    int *equipments_len) {
//   printf("insert a character: ");

//   character_t c;
//   if (scanf("%d %s %s %d %d %d %d %d %d", &c.id, c.name, c.class,
//   &c.stats.hp,
//             &c.stats.mp, &c.stats.atk, &c.stats.def, &c.stats.mag,
//             &c.stats.spr) != 9) {
//     printf("bad format\n");
//     return;
//   };

//   // Allocate its equipment
//   if (*equipments == NULL) {
//     *equipments = calloc(1, sizeof(equipment_t));
//   } else {
//     *equipments = realloc(*equipments, *equipments_len + 1);
//     (*equipments)[*equipments_len].in_use = false;
//   }

//   // and clear it
//   for (size_t i = 0; i < 8; i++) {
//     (*equipments)[*equipments_len].equipped[i] = NULL;
//   }

//   // update the character's equipment
//   c.equipment = *equipments_len;

//   // update the total equipments length
//   *equipments_len = (*equipments_len) + 1;

//   // update the character list
//   *characters = insert_character_linked_list(*characters, c);
// }

// void character_add_object_from_inventory(character_list_t *characters,
//                                          equipment_t **equipments,
//                                          int *equipments_len,
//                                          inventory_t *inventory) {
//   printf("insert character_t code to select: ");
//   int id;
//   scanf("%d", &id);
//   printf("insert object name to equip: ");
//   char object_name[50 + 1];
//   scanf("%s", object_name);

//   character *cp = find_character_in_linked_list(*characters, id);
//   if (cp == NULL) {
//     printf("Unknown character\n");
//     return;
//   }
//   object *op = inventory_find_object_by_name(object_name, inventory);
//   if (op == NULL) {
//     printf("Unknown object\n");
//     return;
//   }
//   if
//   (!insert_into_null_terminated_array((*equipments)[cp->equipment].equipped,
//                                          op)) {
//     printf("Failed to insert\n");
//     return;
//   }
// };

// void character_remove_object_from_inventory(character_list_t *characters,
//                                             equipment_t **equipments,
//                                             int *equipments_len,
//                                             inventory_t *inventory) {
//   printf("insert character_t code to select: ");
//   int id;
//   scanf("%d", &id);
//   printf("insert object name to unequip: ");
//   char object_name[50 + 1];
//   scanf("%s", object_name);

//   character *cp = find_character_in_linked_list(*characters, id);
//   if (cp == NULL) {
//     printf("Unknown character\n");
//   }
//   object *op = inventory_find_object_by_name(object_name, inventory);
//   if (op == NULL) {
//     printf("Unknown object\n");
//   }
//   if
//   (!remove_from_null_terminated_array((*equipments)[cp->equipment].equipped,
//                                          op)) {
//     printf("Failed to remove\n");
//   }
// };