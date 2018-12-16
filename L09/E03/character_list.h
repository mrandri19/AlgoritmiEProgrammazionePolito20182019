#ifndef CHARACTER_LIST
#define CHARACTER_LIST

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "character.h"
#include "equipment_array.h"

typedef struct linked_list {
  character_t head;
  struct linked_list *tail;
} linked_list_t;

typedef struct {
  // Pointer to the __first__ element of the linked list
  linked_list_t *first;

  // Pointer to the __last__ element of the linked list;
  linked_list_t *last;
} character_list_t;

character_list_t character_list_from_file(FILE *fp,
                                          equipment_array_t *equipments);

character_list_t character_list_add_character(character_list_t characters,
                                              character_t character);

character_list_t
character_list_remove_character_by_id(character_list_t characters, int id);

character_t character_list_find_character_by_id(character_list_t characters,
                                                int id);

void character_list_print(character_list_t);

character_list_t character_list_new();

void character_list_free(character_list_t);

#endif