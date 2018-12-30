#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// back, front are acrobatic
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

int difficulty(element_t *diagonal, size_t diagonal_len) {
  int difficulty = 0;
  for (size_t i = 0; i < diagonal_len; ++i) {
    difficulty += diagonal[i].difficulty;
  }
  return difficulty;
}
float value(element_t *diagonal, size_t diagonal_len) {
  float difficulty = 0;
  for (size_t i = 0; i < diagonal_len; ++i) {
    difficulty += diagonal[i].value;
  }
  return difficulty;
}

int count_acrobatic_movements(element_t *diagonal, size_t diagonal_len) {
  int count = 0;
  for (size_t i = 0; i < diagonal_len; ++i) {
    if (diagonal[i].type == front || diagonal[i].type == back)
      count++;
  }
  return count;
}

bool program_has_one_forwards(element_t *diagonal, size_t diagonal_len) {
  for (size_t i = 0; i < diagonal_len; ++i) {
    if (diagonal[i].type == front)
      return true;
  }
  return false;
}

bool program_has_one_backwards(element_t *diagonal, size_t diagonal_len) {
  for (size_t i = 0; i < diagonal_len; ++i) {
    if (diagonal[i].type == back)
      return true;
  }
  return false;
}

int pick_best(element_t *diagonal, size_t diagonal_len, element_t *elements,
              size_t elements_len, element_t *program, size_t program_len,
              int DD, int DP) {

  float *scores = calloc(elements_len, sizeof(int));

  for (size_t i = 0; i < elements_len; ++i) {
    float score = 0;

    // Diagonal difficulty must be less than DD
    if ((DD - difficulty(diagonal, diagonal_len)) / 2 <
        elements[i].difficulty) {
      score -= 2147483647;
    }
    // Program difficulty must be less than DP
    if ((DP - difficulty(program, program_len)) / 2 < elements[i].difficulty) {
      score -= 2147483647;
    }

    if (i == 0) {
      // Must start forwards
      if (elements[0].enter_direction != forwards) {
        score -= 2147483647;
      }
    } else {
      // The element must have enter direction equal to the exit direction of
      // the previous
      if (elements[i - 1].exit_direction != elements[i].enter_direction) {
        score -= 2147483647;
      }
    }

    // Must include at least one acrobatic element
    if (count_acrobatic_movements(diagonal, diagonal_len) < 1) {
      if (elements[i].type == back || elements[i].type == front) {
        score += 1000;
      }
    }

    // Must include at least one forwards and backwards in the whole program
    if (!program_has_one_forwards(program, program_len)) {
      if (elements[i].type == front) {
        score += 500;
      }
    }
    if (!program_has_one_backwards(program, program_len)) {
      if (elements[i].type == back) {
        score += 500;
      }
    }

    // Must at least have two acrobatic movement in sequence in the whole
    // program
    if (count_acrobatic_movements(program, program_len) < 2) {
      if (elements[i].type == back || elements[i].type == front) {
        score += 500;
      }
    }

    // Try to get the item with the highest value
    score += elements[i].value;

    scores[i] = score;
  }

  bool did_we_take_one = false;
  int best = 0;
  for (int i = 0; i < (int)elements_len; ++i) {
    if (scores[best] < scores[i]) {
      best = i;
      did_we_take_one = true;
    }
  }

  free(scores);

  if (did_we_take_one) {
    return best;
  } else {
    return -1;
  }
}

int main(int argc, char **argv) {
  if (argc != 3) {
    printf("Usage %s DD DP\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  FILE *fp = fopen("elementi.txt", "r");
  if (fp == NULL) {
    perror("cannot open elementi.txt");
    exit(EXIT_FAILURE);
  }

  size_t elements_len;
  fscanf(fp, "%zu ", &elements_len);
  element_t *elements = calloc(elements_len, sizeof(element_t));

  for (size_t i = 0; i < elements_len; i++) {
    elements[i] = element_read(fp);
  }

  int DD = atoi(argv[1]);
  int DP = atoi(argv[2]);

  size_t program_len = 0;
  element_t *program = calloc(15, sizeof(element_t));

  for (int j = 0; j < 3; ++j) {
    // Calculate for a single diagonal
    size_t diagonal_len = 0;
    element_t *diagonal = calloc(5, sizeof(element_t));

    while (true) {
      if (diagonal_len == 5)
        break;
      if (difficulty(diagonal, diagonal_len) > DD) {
        printf("Diagonal difficulty excessive: %d\n",
               difficulty(diagonal, diagonal_len));
        exit(EXIT_FAILURE);
      }
      if (difficulty(program, program_len) > DP) {
        printf("Program difficulty excessive: %d\n",
               difficulty(program, program_len));
        exit(EXIT_FAILURE);
      }

      // Pick the best element using the already chosen to guide the search
      int chosen = pick_best(diagonal, diagonal_len, elements, elements_len,
                             program, program_len, DD, DP);
      if (chosen == -1) {
        break;
      }
      diagonal[diagonal_len] = elements[chosen];
      program[program_len] = diagonal[diagonal_len];
      diagonal_len++;
      program_len++;
    }

    printf("Diagonal #%d, total difficulty: %d, total value: %f\n", j,
           difficulty(diagonal, diagonal_len), value(diagonal, diagonal_len));
    for (size_t i = 0; i < diagonal_len; ++i) {
      element_print(diagonal[i]);
    }
    printf("\n");
    free(diagonal);
  }

  printf("Program, total difficulty: %d, total value: %f\n",
         difficulty(program, program_len), value(program, program_len));
  for (size_t i = 0; i < program_len; ++i) {
    element_print(program[i]);
  }

  free(program);

  free(elements);
  fclose(fp);
  return 0;
}
