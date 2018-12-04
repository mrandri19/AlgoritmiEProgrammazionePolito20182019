#ifndef INVENTORY
#define INVENTORY

typedef struct {
  int hp;
  int mp;
  int atk;
  int def;
  int mag;
  int spr;
} stats_t;

typedef struct {
  char name[50 + 1];
  char type[50 + 1];
  stats_t stats;
} object;

typedef struct {
  bool in_use;

  // NULL terminated array
  object *equipped[8];
} equipment_t;

typedef struct {
  object *objects;
  int size;
  int capacity;
} inventory_t;

inventory_t inventory_empty();
void inventory_print(inventory_t *inventory);
void inventory_load_objects(FILE *fp, inventory_t *ow);
object *inventory_find_object_by_name(char *name, inventory_t *inventory);

void print_object(object o);

void print_equipments_array(equipment_t *equipments, size_t equipments_len);

bool insert_into_null_terminated_array(object *array[], object *o);
bool remove_from_null_terminated_array(object *array[], object *o);

#endif