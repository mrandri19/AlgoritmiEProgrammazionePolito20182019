#include <stdio.h>
#include <assert.h>

#include "character.h"
#include "character_list.h"

void character_print(character_t c) {
  printf("PG%d %s %s equipment(%d) %d %d %d %d %d %d\n", c.id, c.name, c.class,
         c.equipment, c.stats.hp, c.stats.mp, c.stats.atk, c.stats.def,
         c.stats.mag, c.stats.spr);
}

void character_print_with_equipment(character_t c, equipment_array_t equipments,
                                    inventory_array_t inventory) {
  int hp = c.stats.hp;
  int mp = c.stats.mp;
  int atk = c.stats.atk;
  int def = c.stats.def;
  int mag = c.stats.mag;
  int spr = c.stats.spr;

  equipment_t equipment =
      equipments_array_get_equipment_by_id(equipments, c.equipment);

  int *object_ids = equipment_get_all_ids(&equipment);

  // TODO: finish
  for (size_t i = 0; i < 8 && object_ids[i] != -1; i++) {
    printf("%d\n", object_ids[i]);
  }

  // int i = 0;
  // while (i < 8) {
  //   object *op = equipments[c->equipment].equipped[i];
  //   if (op == NULL) {
  //     break;
  //   }
  //   hp += op->stats.hp;
  //   mp += op->stats.mp;
  //   atk += op->stats.atk;
  //   def += op->stats.def;
  //   mag += op->stats.mag;
  //   spr += op->stats.spr;
  //   i++;
  // }

  printf("PG%d %s %s equipment(%d) %d %d %d %d %d %d\n", c.id, c.name, c.class,
         c.equipment, MAX(0, hp), MAX(0, mp), MAX(0, atk), MAX(0, def),
         MAX(0, mag), MAX(0, spr));
}

character_t character_load_from_keyboard() {
  character_t c;
  scanf("PG%d %s %s %d %d %d %d %d %d ", &c.id, c.name, c.class, &c.stats.hp,
        &c.stats.mp, &c.stats.atk, &c.stats.def, &c.stats.mag, &c.stats.spr);

  return c;
}

void character_add_object_from_inventory(equipment_t c,
                                         char *equipments,
                                         inventory_array_t name
) {
    assert(false);
}

void character_remove_object_from_inventory(equipment_t eq, char name[256], inventory_array_t inventory) {
  assert(false);
}

size_t character_get_equipment_id(character_t c) {
  return c.equipment;
}

void character_set_equipment(character_t *ptr, size_t id) {
    ptr->equipment = id;
}