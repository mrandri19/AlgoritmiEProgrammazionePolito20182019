#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;
    char name[50 + 1];
    char class[50 + 1];
    int hp;
    int mp;
    int atk;
    int def;
    int mag;
    int spr;
} character;

typedef struct {
    char name[50 + 1];
    char type[50 + 1];
    int hp;
    int mp;
    int atk;
    int def;
    int mag;
    int spr;
} object;

int main()
{
    FILE* fp_characters = fopen("pg.txt", "r");
    if (fp_characters == NULL) {
        perror("Cannot open pg.txt");
        return 1;
    }

    int characters_len;
    fscanf(fp_characters, "%d ", &characters_len);

    character* characters = calloc(characters_len, sizeof(character));
    for (int i = 0; i < characters_len; i++) {
        fscanf(fp_characters, "PG%d %s %s %d %d %d %d %d %d ",
            &characters[i].id,
            characters[i].name,
            characters[i].class,
            &characters[i].hp,
            &characters[i].mp,
            &characters[i].atk,
            &characters[i].def,
            &characters[i].mag,
            &characters[i].spr);
    }

    // for (int i = 0; i < characters_len; i++) {
    //     printf("PG%d %s %s %d %d %d %d %d %d\n",
    //         characters[i].id,
    //         characters[i].name,
    //         characters[i].class,
    //         characters[i].hp,
    //         characters[i].mp,
    //         characters[i].atk,
    //         characters[i].def,
    //         characters[i].mag,
    //         characters[i].spr);
    // }

    FILE* fp_objects = fopen("inventario.txt", "r");
    if (fp_objects == NULL) {
        perror("Cannot open inventario.txt");
        return 1;
    }
    int objects_len;
    fscanf(fp_objects, "%d ", &objects_len);

    object* objects = calloc(objects_len, sizeof(object));
    for (int i = 0; i < objects_len; i++) {
        fscanf(fp_objects, "%s %s %d %d %d %d %d %d ",
            objects[i].name,
            objects[i].type,
            &objects[i].hp,
            &objects[i].mp,
            &objects[i].atk,
            &objects[i].def,
            &objects[i].mag,
            &objects[i].spr);
    }

    // for (int i = 0; i < objects_len; i++) {
    //     printf("%s %s %d %d %d %d %d %d\n",
    //         objects[i].name,
    //         objects[i].type,
    //         objects[i].hp,
    //         objects[i].mp,
    //         objects[i].atk,
    //         objects[i].def,
    //         objects[i].mag,
    //         objects[i].spr);
    // }

    // TODO: finish

    free(objects);
    fclose(fp_objects);

    free(characters);
    fclose(fp_characters);

    return 0;
}
