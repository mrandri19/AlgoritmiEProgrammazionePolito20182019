#ifndef CHARACTERS
#define CHARACTERS

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#include "inventory.h"

typedef struct {
  int id;
  char name[50 + 1];
  char class[50 + 1];
  int equipment; // Offset in the equipment vector
  stats_t stats;
} character;

typedef struct linked_list {
  character head;
  struct linked_list *tail;
} linked_list_t;

typedef struct {
  linked_list_t *first; // Pointer to the __first__ element of the linked list
  linked_list_t *last;  // Pointer to the __last__ element of the linked list;
} characters_t;

void print_character(character c);

void print_character_with_equipment(character *cp, equipment_t *equipments);

linked_list_t *new_character_linked_list();

linked_list_t *insert_character_linked_list(linked_list_t *list, character c);

void free_linked_list(linked_list_t *list);

void print_character_linked_list(linked_list_t *list);

linked_list_t *remove_character_id_linked_list(linked_list_t *list, int id);

character *find_character_in_linked_list(linked_list_t *list, int id);

linked_list_t *last_element_linked_list(linked_list_t *list);

void load_characters_from_file(FILE *fp_characters, linked_list_t **characters,
                               equipment_t **equipments, int *equipments_len);

void load_characters_from_keyboard(linked_list_t **characters,
                                   equipment_t **equipments,
                                   int *equipments_len);

void character_add_object_from_inventory(linked_list_t **characters,
                                         equipment_t **equipments,
                                         int *equipments_len,
                                         inventory_t *inventory);

void character_remove_object_from_inventory(linked_list_t **characters,
                                            equipment_t **equipments,
                                            int *equipments_len,
                                            inventory_t *inventory);
#endif