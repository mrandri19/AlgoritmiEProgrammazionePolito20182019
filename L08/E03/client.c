#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "characters.h"
#include "inventory.h"

#define COMMAND_BUF 256
#define DEBUG true

typedef enum {
  load_characters,
  load_objects,
  add_character,
  remove_character,
  add_object_from_inventory,
  remove_object_from_inventory,
  stats,
  unknown
} command;

command parse_command(char *command_s) {
  if (strcmp(command_s, "lc") == 0) {
    return load_characters;
  }
  if (strcmp(command_s, "lo") == 0) {
    return load_objects;
  }
  if (strcmp(command_s, "ac") == 0) {
    return add_character;
  }
  if (strcmp(command_s, "rc") == 0) {
    return remove_character;
  }
  if (strcmp(command_s, "ao") == 0) {
    return add_object_from_inventory;
  }
  if (strcmp(command_s, "ro") == 0) {
    return remove_object_from_inventory;
  }
  if (strcmp(command_s, "st") == 0) {
    return stats;
  }
  return unknown;
}

int main() {
  characters_t characters;
  linked_list_t *characters_bad = new_character_linked_list();
  characters.first = characters_bad;
  characters.last = characters_bad;

  int equipments_len = 0;
  equipment_t *equipments = NULL;

  inventory_t inventory = inventory_empty();

  while (true) {
    char command[COMMAND_BUF];
    printf("> ");
    if (scanf("%s", command) == EOF) {
      printf("\n");
      break;
    }

    switch (parse_command(command)) {
    case load_characters: {
      FILE *fp_characters = fopen("pg.txt", "r");
      if (fp_characters == NULL) {
        perror("Cannot open pg.txt");
        return 1;
      }

      load_characters_from_file(fp_characters, &characters_bad, &equipments,
                                &equipments_len);
      characters.first = characters_bad;
      characters.last = last_element_linked_list(characters_bad);

      fclose(fp_characters);
      break;
    }
    case load_objects: {
      FILE *fp_objects = fopen("inventario.txt", "r");
      if (fp_objects == NULL) {
        perror("Cannot open inventario.txt");
        return 1;
      }

      inventory_load_objects(fp_objects, &inventory);

      fclose(fp_objects);
      break;
    }
    case add_character: {
      load_characters_from_keyboard(&characters_bad, &equipments,
                                    &equipments_len);
      break;
    }
    case remove_character: {
      // TODO: this keeps his equipment into the equipment array, what
      // should we do?
      printf("insert character code to remove: ");
      int id;
      scanf("%d", &id);
      characters_bad = remove_character_id_linked_list(characters_bad, id);
      break;
    }
    case add_object_from_inventory: {
      character_add_object_from_inventory(&characters_bad, &equipments,
                                          &equipments_len, &inventory);
      break;
    }
    case remove_object_from_inventory: {
      character_remove_object_from_inventory(&characters_bad, &equipments,
                                             &equipments_len, &inventory);
      break;
    }
    case stats: {
      printf("Insert a character's id whose stats you want: ");
      int id;
      scanf("%d", &id);
      character *cp = find_character_in_linked_list(characters_bad, id);
      if (cp == NULL) {
        printf("Unknown character");
        break;
      }
      printf("Character without equipment:\n");
      print_character(*cp);

      printf("Character with equipment:\n");
      print_character_with_equipment(cp, equipments);

      break;
    }
    case unknown:
      printf("unknown command\n");
    }

#if DEBUG
    printf("\n**** DEBUG ****\n");
    print_character_linked_list(characters_bad);
    print_equipments_array(equipments, equipments_len);
    inventory_print(&inventory);
    printf("**** DEBUG ****\n");
#endif
  }

  free(inventory.objects);

  free(equipments);
  free_linked_list(characters_bad);

  return 0;
}