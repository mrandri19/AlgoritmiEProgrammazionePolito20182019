#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inventory.h"

void object_print(object_t o) {
  printf("%s %s %d %d %d %d %d %d\n", o.name, o.type, o.stats.hp, o.stats.mp,
         o.stats.atk, o.stats.def, o.stats.mag, o.stats.spr);
}
