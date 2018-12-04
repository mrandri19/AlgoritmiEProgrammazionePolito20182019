// TODO: figure out what to do with it
// provably just put into include guards
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "characters.h"

void print_character(character c) {
  printf("PG%d %s %s equipment(%d) %d %d %d %d %d %d\n", c.id, c.name, c.class,
         c.equipment, c.stats.hp, c.stats.mp, c.stats.atk, c.stats.def,
         c.stats.mag, c.stats.spr);
}

void print_character_with_equimpment(character *cp, equipment_t *equipments) {
  int hp = cp->stats.hp;
  int mp = cp->stats.mp;
  int atk = cp->stats.atk;
  int def = cp->stats.def;
  int mag = cp->stats.mag;
  int spr = cp->stats.spr;

  int i = 0;
  while (i < 8) {
    object *op = equipments[cp->equipment].equipped[i];
    if (op == NULL) {
      break;
    }
    hp += op->stats.hp;
    mp += op->stats.mp;
    atk += op->stats.atk;
    def += op->stats.def;
    mag += op->stats.mag;
    spr += op->stats.spr;
    i++;
  }

  printf("PG%d %s %s equipment(%d) %d %d %d %d %d %d\n", cp->id, cp->name,
         cp->class, cp->equipment, MAX(0, hp), MAX(0, mp), MAX(0, atk),
         MAX(0, def), MAX(0, mag), MAX(0, spr));
}
linked_list_t *new_character_linked_list() { return NULL; }

linked_list_t *insert_character_linked_list(linked_list_t *list, character c) {
  if (list == NULL) {
    linked_list_t *l = calloc(1, sizeof(linked_list_t));
    l->head = c;
    l->tail = NULL;
    return l;
  } else {
    list->tail = insert_character_linked_list(list->tail, c);
    return list;
  }
}

void free_linked_list(linked_list_t *list) {
  if (list == NULL) {
    return;
  } else {
    free_linked_list(list->tail);
    free(list);
  }
}

void print_character_linked_list(linked_list_t *list) {
  if (list == NULL)
    return;
  character c = list->head;

  print_character(c);
  print_character_linked_list(list->tail);
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

character *find_character_in_linked_list(linked_list_t *list, int id) {
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

linked_list_t *last_element_linked_list(linked_list_t *list) {
  if (list == NULL) {
    exit(1);
  }
  if (list->tail == NULL) {
    return list;
  } else {
    return last_element_linked_list(list->tail);
  }
}

void load_characters_from_file(FILE *fp_characters, linked_list_t **characters,
                               equipment_t **equipments, int *equipments_len) {
  int characters_len;
  fscanf(fp_characters, "%d ", &characters_len);

  // Allocate the equipment vector for each character
  int first_free_index = 0;
  if (*equipments_len == 0) {
    *equipments_len = characters_len;
    *equipments = calloc(characters_len, sizeof(equipment_t));
  } else {
    *equipments_len += characters_len;
    *equipments = realloc(*equipments, *equipments_len * sizeof(equipment_t));
    first_free_index = characters_len;
  }

  // then for each character
  for (int i = 0; i < characters_len; i++) {
    character c;
    fscanf(fp_characters, "PG%d %s %s %d %d %d %d %d %d ", &c.id, c.name,
           c.class, &c.stats.hp, &c.stats.mp, &c.stats.atk, &c.stats.def,
           &c.stats.mag, &c.stats.spr);

    // clear its equipment
    c.equipment = first_free_index + i;
    (*equipments)[i].in_use = false;
    for (size_t j = 0; j < 8; j++) {
      (*equipments)[i].equipped[j] = NULL;
    }

    *characters = insert_character_linked_list(*characters, c);
  }
}

void load_characters_from_keyboard(linked_list_t **characters,
                                   equipment_t **equipments,
                                   int *equipments_len) {
  printf("insert a character: ");

  character c;
  if (scanf("%d %s %s %d %d %d %d %d %d", &c.id, c.name, c.class, &c.stats.hp,
            &c.stats.mp, &c.stats.atk, &c.stats.def, &c.stats.mag,
            &c.stats.spr) != 9) {
    printf("bad format\n");
    return;
  };

  // Allocate its equipment
  if (*equipments == NULL) {
    *equipments = calloc(1, sizeof(equipment_t));
  } else {
    *equipments = realloc(*equipments, *equipments_len + 1);
    (*equipments)[*equipments_len].in_use = false;
  }

  // and clear it
  for (size_t i = 0; i < 8; i++) {
    (*equipments)[*equipments_len].equipped[i] = NULL;
  }

  // update the character's equipment
  c.equipment = *equipments_len;

  // update the total equipments length
  *equipments_len = (*equipments_len) + 1;

  // update the character list
  *characters = insert_character_linked_list(*characters, c);
}

void character_add_object_from_inventory(linked_list_t **characters,
                                         equipment_t **equipments,
                                         int *equipments_len,
                                         inventory_t *inventory) {
  printf("insert character code to select: ");
  int id;
  scanf("%d", &id);
  printf("insert object name to equip: ");
  char object_name[50 + 1];
  scanf("%s", object_name);

  character *cp = find_character_in_linked_list(*characters, id);
  if (cp == NULL) {
    printf("Unknown character\n");
    return;
  }
  object *op = inventory_find_object_by_name(object_name, inventory);
  if (op == NULL) {
    printf("Unknown object\n");
    return;
  }
  if (!insert_into_null_terminated_array((*equipments)[cp->equipment].equipped,
                                         op)) {
    printf("Failed to insert\n");
    return;
  }
};

void character_remove_object_from_inventory(linked_list_t **characters,
                                            equipment_t **equipments,
                                            int *equipments_len,
                                            inventory_t *inventory) {
  printf("insert character code to select: ");
  int id;
  scanf("%d", &id);
  printf("insert object name to unequip: ");
  char object_name[50 + 1];
  scanf("%s", object_name);

  character *cp = find_character_in_linked_list(*characters, id);
  if (cp == NULL) {
    printf("Unknown character\n");
  }
  object *op = inventory_find_object_by_name(object_name, inventory);
  if (op == NULL) {
    printf("Unknown object\n");
  }
  if (!remove_from_null_terminated_array((*equipments)[cp->equipment].equipped,
                                         op)) {
    printf("Failed to remove\n");
  }
};