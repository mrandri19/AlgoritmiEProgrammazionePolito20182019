#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COMMAND_BUF 256
#define DEBUG true

#define MAX(a, b) (((a) > (b)) ? (a) : (b))

// TODO: remove magic numbers (50+1), (8)

/* ************************************************************************* */

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
    object* equipped[8]; // NULL terminated array
} equipment_t;

typedef struct {
    int id;
    char name[50 + 1];
    char class[50 + 1];
    int equipment; // Offset in the equipment vector
    stats_t stats;
} character;

/* ************************************************************************* */

void print_character(character c)
{
    printf("PG%d %s %s equipment(%d) %d %d %d %d %d %d\n",
        c.id,
        c.name,
        c.class,
        c.equipment,
        c.stats.hp,
        c.stats.mp,
        c.stats.atk,
        c.stats.def,
        c.stats.mag,
        c.stats.spr);
}

void print_character_with_equimpment(character* cp, equipment_t* equipments)
{
    int hp = cp->stats.hp;
    int mp = cp->stats.mp;
    int atk = cp->stats.atk;
    int def = cp->stats.def;
    int mag = cp->stats.mag;
    int spr = cp->stats.spr;

    int i = 0;
    while (i < 8) {
        object* op = equipments[cp->equipment].equipped[i];
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

    printf("PG%d %s %s equipment(%d) %d %d %d %d %d %d\n",
        cp->id,
        cp->name,
        cp->class,
        cp->equipment,
        MAX(0, hp),
        MAX(0, mp),
        MAX(0, atk),
        MAX(0, def),
        MAX(0, mag),
        MAX(0, spr));
}

void print_object(object o)
{
    printf("%s %s %d %d %d %d %d %d\n",
        o.name,
        o.type,
        o.stats.hp,
        o.stats.mp,
        o.stats.atk,
        o.stats.def,
        o.stats.mag,
        o.stats.spr);
}

void print_equipments_array(equipment_t* equipments, size_t equipments_len)
{

    for (size_t i = 0; i < equipments_len; i++) {
        printf("[%zu] = ", i);
        int j = 0;
        while (j < 8) {
            object* item = equipments[i].equipped[j];
            if (item == NULL)
                break;
            printf("[%p], ", (void*)item);
            j++;
        }
        printf("\n");
    }
}

void print_objects_array(object* objects, size_t objects_len)
{
    for (size_t i = 0; i < objects_len; i++) {
        print_object(objects[i]);
    }
}
/* ************************************************************************* */

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

command parse_command(char* command_s)
{
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

/* ************************************************************************* */

typedef struct linked_list {
    character head;
    struct linked_list* tail;
} linked_list_t;

linked_list_t* new_character_linked_list()
{
    return NULL;
}

linked_list_t* insert_character_linked_list(linked_list_t* list, character c)
{
    if (list == NULL) {
        linked_list_t* l = calloc(1, sizeof(linked_list_t));
        l->head = c;
        l->tail = NULL;
        return l;
    } else {
        list->tail = insert_character_linked_list(list->tail, c);
        return list;
    }
}

void free_linked_list(linked_list_t* list)
{
    if (list == NULL) {
        return;
    } else {
        free_linked_list(list->tail);
        free(list);
    }
}

void print_character_linked_list(linked_list_t* list)
{
    if (list == NULL)
        return;
    character c = list->head;

    print_character(c);
    print_character_linked_list(list->tail);
}

linked_list_t* remove_character_id_linked_list(linked_list_t* list, int id)
{
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

character* find_character_in_linked_list(linked_list_t* list, int id)
{
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

linked_list_t* last_element_linked_list(linked_list_t* list)
{
    if (list == NULL) {
        exit(1);
    }
    if (list->tail == NULL) {
        return list;
    } else {
        return last_element_linked_list(list->tail);
    }
}

/* ************************************************************************* */

object* find_object_by_name_in_array(char* name, object* objects, size_t objects_len)
{
    for (size_t i = 0; i < objects_len; i++) {
        if (strcmp(objects[i].name, name) == 0) {
            return &objects[i];
        }
    }
    return NULL;
}

bool insert_into_null_terminated_array(object* array[], object* o)
{
    int i = 0;
    while (i < 8) {
        if (array[i] == NULL) {
            array[i] = o;
            return true;
        } else {
            i++;
        }
    }
    return false;
}

bool remove_from_null_terminated_array(object* array[], object* o)
{
    int i = 0;
    int pos = -1;
    while (array[i] != NULL && i < 8) {
        if (strcmp(array[i]->name, o->name) == 0) {
            pos = i;
            break;
        }
    }

    if (pos == -1) {
        // object not found
        return false;
    }
    array[i] = NULL;

    // Compact the array (remove the newly created null pointer ) i.e.
    // {1, 2, NULL, 3, NULL, NULL} -> {1, 2, 3, NULL, NULL}

    i = 0;
    while (i < 8 - 1) {
        if (array[i] == NULL && array[i + 1] != NULL) {
            array[i] = array[i + 1];
            array[i + 1] = NULL;
        }
        i++;
    }
    return true;
}

/* ************************************************************************* */

typedef struct {
    linked_list_t* first; // Pointer to the __first__ element of the linked list
    linked_list_t* last; // Pointer to the __last__ element of the linked list;
} characterWrapper_t;

typedef struct {
    object* objects;
    int objects_len;
    int max_objects_len;
} objectWrapper_t;

/* ************************************************************************* */

int main()
{
    characterWrapper_t cw;
    linked_list_t* characters = new_character_linked_list();
    cw.first = characters;
    cw.last = characters;

    int equipments_len = 0;
    equipment_t* equipments = NULL;

    objectWrapper_t ow;
    ow.objects = NULL;
    ow.objects_len = 0;
    ow.max_objects_len = 0;

    while (true) {
        char command[COMMAND_BUF];
        printf("> ");
        if (scanf("%s", command) == EOF) {
            printf("\n");
            break;
        }

        switch (parse_command(command)) {
        case load_characters: {
            FILE* fp_characters = fopen("pg.txt", "r");
            if (fp_characters == NULL) {
                perror("Cannot open pg.txt");
                return 1;
            }

            int characters_len;
            fscanf(fp_characters, "%d ", &characters_len);

            // Allocate the equipment vector for each character
            equipments_len = characters_len;
            equipments = calloc(characters_len, sizeof(equipment_t));

            for (int i = 0; i < characters_len; i++) {
                character c;
                fscanf(fp_characters, "PG%d %s %s %d %d %d %d %d %d ",
                    &c.id,
                    c.name,
                    c.class,
                    &c.stats.hp,
                    &c.stats.mp,
                    &c.stats.atk,
                    &c.stats.def,
                    &c.stats.mag,
                    &c.stats.spr);

                // And clear the equipment for each character
                c.equipment = i;
                equipments[i].in_use = false;
                for (size_t j = 0; j < 8; j++) {
                    equipments[i].equipped[j] = NULL;
                }

                characters = insert_character_linked_list(characters, c);
            }

            cw.first = characters;
            cw.last = last_element_linked_list(characters);

            fclose(fp_characters);
            break;
        }
        case load_objects: {
            FILE* fp_objects = fopen("inventario.txt", "r");
            if (fp_objects == NULL) {
                perror("Cannot open inventario.txt");
                return 1;
            }

            fscanf(fp_objects, "%d ", &ow.objects_len);
            ow.objects = calloc(ow.objects_len, sizeof(object));

            for (int i = 0; i < ow.objects_len; i++) {
                fscanf(fp_objects, "%s %s %d %d %d %d %d %d ",
                    ow.objects[i].name,
                    ow.objects[i].type,
                    &ow.objects[i].stats.hp,
                    &ow.objects[i].stats.mp,
                    &ow.objects[i].stats.atk,
                    &ow.objects[i].stats.def,
                    &ow.objects[i].stats.mag,
                    &ow.objects[i].stats.spr);
            }

            fclose(fp_objects);
            break;
        }
        case add_character: {
            printf("insert a character: ");

            character c;
            scanf("%d %s %s %d %d %d %d %d %d ",
                &c.id,
                c.name,
                c.class,
                &c.stats.hp,
                &c.stats.mp,
                &c.stats.atk,
                &c.stats.def,
                &c.stats.mag,
                &c.stats.spr);

            // Allocate its equipment and clear it
            equipments = realloc(equipments, equipments_len + 1);
            equipments[equipments_len].in_use = false;
            for (size_t i = 0; i < 8; i++) {
                equipments[equipments_len].equipped[i] = NULL;
            }
            c.equipment = equipments_len;
            equipments_len++;

            characters = insert_character_linked_list(characters, c);
            cw.last = last_element_linked_list(characters);

            break;
        }
        case remove_character: {
            // TODO: this keeps his equipment into the equipment array, what
            // should we do?
            printf("insert character code to remove: ");
            int id;
            scanf("%d", &id);
            characters = remove_character_id_linked_list(characters, id);
            break;
        }
        case add_object_from_inventory: {
            printf("insert character code to select: ");
            int id;
            scanf("%d", &id);
            printf("insert object name to equip: ");
            char object_name[50 + 1];
            scanf("%s", object_name);

            character* cp = find_character_in_linked_list(characters, id);
            if (cp == NULL) {
                printf("Unknown character\n");
                break;
            }
            object* op = find_object_by_name_in_array(object_name, ow.objects, ow.objects_len);
            if (op == NULL) {
                printf("Unknown object\n");
                break;
            }
            if (!insert_into_null_terminated_array(equipments[cp->equipment].equipped, op)) {
                printf("Failed to insert\n");
                break;
            }
            break;
        }
        case remove_object_from_inventory: {
            printf("insert character code to select: ");
            int id;
            scanf("%d", &id);
            printf("insert object name to unequip: ");
            char object_name[50 + 1];
            scanf("%s", object_name);

            character* cp = find_character_in_linked_list(characters, id);
            if (cp == NULL) {
                printf("Unknown character\n");
            }
            object* op = find_object_by_name_in_array(object_name, ow.objects, ow.objects_len);
            if (op == NULL) {
                printf("Unknown object\n");
            }
            if (!remove_from_null_terminated_array(equipments[cp->equipment].equipped, op)) {
                printf("Failed to remove\n");
            }
            break;
        }
        case stats: {
            printf("Insert a character's id whose stats you want: ");
            int id;
            scanf("%d", &id);
            character* cp = find_character_in_linked_list(characters, id);
            if (cp == NULL) {
                printf("Unknown character");
                break;
            }
            printf("Character without equipment:\n");
            print_character(*cp);

            printf("Character with equipment:\n");
            print_character_with_equimpment(cp, equipments);

            break;
        }
        case unknown:
            printf("unknown command\n");
        }

#if DEBUG
        printf("\n\n\n");
        print_character_linked_list(characters);
        print_equipments_array(equipments, equipments_len);
        print_objects_array(ow.objects, ow.objects_len);
#endif
    }

    free(ow.objects);

    free(equipments);
    free_linked_list(characters);

    return 0;
}
