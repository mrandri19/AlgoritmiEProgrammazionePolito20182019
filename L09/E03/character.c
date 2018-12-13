#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "character.h"
#include "equipment_array.h"

void character_print(character_t c) {
  printf("PG%d %s %s equipment(%d) %d %d %d %d %d %d\n", c.id, c.name, c.class,
         c.equipment, c.stats.hp, c.stats.mp, c.stats.atk, c.stats.def,
         c.stats.mag, c.stats.spr);
}

void character_print_with_equipment(character_t c,
                                    equipment_array_t equipments) {
  int hp = c.stats.hp;
  int mp = c.stats.mp;
  int atk = c.stats.atk;
  int def = c.stats.def;
  int mag = c.stats.mag;
  int spr = c.stats.spr;

  // TODO: finish
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

character_t character_load_from_keyboard(equipment_array_t *equipments) {
  character_t c;
  scanf("PG%d %s %s %d %d %d %d %d %d ", &c.id, c.name, c.class, &c.stats.hp,
        &c.stats.mp, &c.stats.atk, &c.stats.def, &c.stats.mag, &c.stats.spr);
  c.equipment = equipment_array_add_equipment_for_character(equipments);

  return c;
}

//   character_t c;
//   if (scanf("%d %s %s %d %d %d %d %d %d", &c.id, c.name, c.class,
//   &c.stats.hp,
//             &c.stats.mp, &c.stats.atk, &c.stats.def, &c.stats.mag,
//             &c.stats.spr) != 9) {
//     printf("bad format\n");
//     return;
//   };
