#include <stdio.h>
#include <stdlib.h>

typedef struct {
  char name[101];
  int type;
  int enter_direction;
  int exit_direction;
  int precedence;
  int final;
  float value;
  int difficulty;
} element_t;

element_t element_read(FILE *fp) {
  element_t e;
  fscanf(fp, "%s %d %d %d %d %d %f %d ", e.name, &e.type, &e.enter_direction,
         &e.exit_direction, &e.precedence, &e.final, &e.value, &e.difficulty);
  return e;
}

void element_print(element_t e) {
  printf("%s %d %d %d %d %d %.2f %d\n", e.name, e.type, e.enter_direction,
         e.exit_direction, e.precedence, e.final, e.value, e.difficulty);
}

int main() {
  FILE *fp = fopen("elementi.txt", "r");
  if (fp == NULL) {
    perror("cannot open elementi.txt");
    exit(EXIT_FAILURE);
  }

  size_t len;
  fscanf(fp, "%zu ", &len);
  element_t *elements = calloc(len, sizeof(element_t));

  for (size_t i = 0; i < len; i++) {
    elements[i] = element_read(fp);
  }

  for (size_t i = 0; i < len; i++) {
    element_print(elements[i]);
  }

  // Generate sequence of elements which satisfies constraints and maximizes
  // value
  // Constraints:
  // - enter direction of the element must be the same as the exit direction of
  // the one before
  // - always start facing forwards
  // 3 diagonals

  // at least 1 acrobatic element of type frontal and of type backwards in
  // program at least 1 diagonal with 2 acrobatic elements in sequence

  // if the last element of the last diagonal is >8 bonus multiplier of 1.5
  // max 5 elements per diagonal.

  // each diagonal must have < DD difficulty
  // total score < DP

  free(elements);
  fclose(fp);
  return 0;
}
