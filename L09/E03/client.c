#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "character_list.h"
#include "character.h"

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
  character_list_t characters = character_list_new();
  inventory_array_t inventory = inventory_array_new();
  equipment_array_t equipments = equipment_array_new();

  while (true) {
    char command[COMMAND_BUF];
    printf("> ");
    if (scanf("%s", command) == EOF) {
      printf("\n");
      break;
    }

    switch (parse_command(command)) {
    case load_characters: {
      FILE *fp = fopen("pg.txt", "r");
      if (fp == NULL) {
        perror("Cannot open pg.txt");
        return 1;
      }

      characters = character_list_from_file(fp, &equipments);

      fclose(fp);
      break;
    }
    case load_objects: {
      FILE *fp = fopen("inventario.txt", "r");
      if (fp == NULL) {
        perror("Cannot open inventario.txt");
        return 1;
      }

      inventory = inventory_array_load_file(fp);

      fclose(fp);
      break;
    }
    case add_character: {
      character_t character = character_load_from_keyboard();

      size_t id = equipment_array_add_equipment_for_character(&equipments);
      character_set_equipment(&character, id);

      character_list_add_character(characters, character);
      break;
    }
    case remove_character: {
      printf("insert character code to remove: ");
      int id;
      scanf("%d", &id);

      character_list_remove_character_by_id(characters, id);
      break;
    }
    case add_object_from_inventory: {
      printf("insert character code to add object: ");
      int id;
      scanf("%d", &id);

      printf("insert object name: ");
      char name[COMMAND_BUF];
      scanf("%s", name);

      character_t c = character_list_find_character_by_id(characters, id);

      size_t eq_id = character_get_equipment_id(c);
      equipment_t eq = equipments_array_get_equipment_by_id(equipments,eq_id);
      character_add_object_from_inventory(eq, name, inventory);

      break;
    }
    case remove_object_from_inventory: {
      printf("insert character code to remove object: ");
      int id;
      scanf("%d", &id);

      printf("insert object name: ");
      char name[COMMAND_BUF];
      scanf("%s", name);

      character_t c = character_list_find_character_by_id(characters, id);


      size_t eq_id = character_get_equipment_id(c);
      equipment_t eq = equipments_array_get_equipment_by_id(equipments,eq_id);
      character_remove_object_from_inventory(eq, name, inventory);

      break;
    }
    case stats: {
      printf("Insert a character's id whose stats you want: ");
      int id;
      scanf("%d", &id);
      character_t character =
          character_list_find_character_by_id(characters, id);

      printf("Character without equipment:\n");
      character_print(character);

      printf("Character with equipment:\n");
      character_print_with_equipment(character, equipments, inventory);

      break;
    }
    case unknown:
      printf("unknown command\n");
    }

#if DEBUG
    printf("\n**** DEBUG ****\n");
    character_list_print(characters);
    printf("\n");
    equipment_array_print(equipments);
    printf("\n");
    inventory_array_print(inventory);
    printf("**** DEBUG ****\n");
#endif
  }

  inventory_array_free(inventory);
  equipment_array_free(equipments);
  character_list_free(characters);

  return 0;
}


