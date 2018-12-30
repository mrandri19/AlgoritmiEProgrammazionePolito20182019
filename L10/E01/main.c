#include <stdio.h>
#include <stdlib.h>

typedef enum { transition, back, front } type_t;
typedef enum { backwards, forwards } direction_t;
typedef enum { can_be_first, cannot_be_first } precedence_t;
typedef enum { can_be_penultimate, must_be_last } final_t;

typedef struct {
  char name[101];
  type_t type;
  direction_t enter_direction;
  direction_t exit_direction;
  precedence_t precedence;
  final_t final;
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

void combinations(size_t pos, element_t *elements, element_t *sol, size_t n,
                  size_t k, size_t start) {
  if (pos == k) {
    printf("Sol: \n");
    for (size_t i = 0; i < k; i++) {
      element_print(sol[i]);
    }
  }

  for (size_t i = start; i < n; ++i) {
    sol[pos] = elements[i];
    combinations(pos + 1, elements, sol, n, k, i + 1);
  }
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

  // Generate sequence of elements which satisfies constraints and maximizes
  // value
  // Constraints:
  // - enter direction of the element must be the same as the exit direction
  // of the one before
  // - always start facing forwards
  // 3 diagonals

  // at least 1 acrobatic element of type frontal and of type backwards in
  // program at least 1 diagonal with 2 acrobatic elements in sequence

  // if the last element of the last diagonal is >8 bonus multiplier of 1.5
  // max 5 elements per diagonal.

  // each diagonal must have < DD difficulty
  // total score < DP

  // Calculate each diagonal
  // Sum from i=1 to 5 of combinations(19,i)
  for (int i = 1; i <= 2; ++i) {
    size_t n = len;
    size_t k = i;

    element_t *sol = calloc(k, sizeof(element_t));

    size_t pos = 0;
    size_t start = 0;

    combinations(pos, elements, sol, n, k, start);

    free(sol);
  }

  free(elements);
  fclose(fp);
  return 0;
}
