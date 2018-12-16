#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN 31

struct symbol_table {
  size_t size;
  size_t capacity;
  char **names;
};

typedef struct symbol_table *symbol_table_t;

symbol_table_t symbol_table_empty() {
  symbol_table_t s = (symbol_table_t)malloc(sizeof(struct symbol_table));
  s->size = 0;
  s->capacity = 0;
  s->names = NULL;
  return s;
}

ssize_t symbol_table_find_by_name(symbol_table_t st, char name[LEN]) {
  // DEBUGG("searching %s in symbol table\n", name);
  for (size_t i = 0; i < st->size; i++) {
    if (strcmp(name, st->names[i]) == 0) {
      return i;
    }
  }

  return -1;
}

void symbol_table_print(symbol_table_t st) {
  printf("size = %zu, capacity = %zu\n", st->size, st->capacity);
  for (size_t i = 0; i < st->size; i++) {
    printf("%s\n", st->names[i]);
  }
}

size_t symbol_table_add(symbol_table_t st, char name[LEN]) {
  // Allocate enough capacity
  if (st->capacity == 0) {
    st->capacity = 1;
    st->names = (char **)calloc(st->capacity, sizeof(char *));
  }
  if (st->size == st->capacity) {
    st->capacity *= 2;
    st->names = (char **)realloc(st->names, st->capacity * sizeof(char *));
    // DEBUGG("reallocating, old capacity = %zu, new = %zu\n", st->capacity / 2,
    //  st->capacity);
  }

  st->names[st->size] = (char *)calloc(LEN, sizeof(char));
  strcpy(st->names[st->size], name);
  st->size++;

  return (st->size - 1);
}

void symbol_table_free(symbol_table_t st) {

  for (size_t i = 0; i < st->size; i++) {
    free(st->names[i]);
  }
  free(st->names);
  free(st);
}

size_t symbol_table_size(symbol_table_t st) { return st->size; }

char *symbol_table_get(symbol_table_t st, size_t index) {
  return st->names[index];
}
