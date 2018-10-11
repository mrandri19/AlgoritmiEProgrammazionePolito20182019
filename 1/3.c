#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RIGHT 1
#define LEFT 2

#define UP 3
#define DOWN 4

void print_matrix(int m[30][30], int rows, int cols) {
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      printf("%d ", m[i][j]);
    }
    printf("\n");
  }
}

void shift_matrix_row(int m[30][30], int rows, int cols, int index,
                      int direction, int times) {
  if (direction == RIGHT) {
  } else {
    times = cols - (times % cols);
  }
  for (int i = 0; i < times; ++i) {
    int a = m[index][0];
    for (int j = 0; j < cols; ++j) {
      int b = m[index][j + 1];
      m[index][(j + 1) % cols] = a;
      a = b;
    }
  }
}

void shift_matrix_col(int m[30][30], int rows, int cols, int index,
                      int direction, int times) {
  if (direction == DOWN) {
  } else {
    times = rows - (times % rows);
  }
  for (int i = 0; i < times; ++i) {
    int a = m[0][index];
    for (int j = 0; j < rows; ++j) {
      int b = m[j + 1][index];
      m[(j + 1) % rows][index] = a;
      a = b;
    }
  }
}

int main() {
  printf("Inserire nome del file: ");
  char filename[20 + 1];
  scanf("%s", filename);

  FILE* fp = fopen(filename, "r");
  if (fp == NULL) {
    perror("Could not open file");
    return -1;
  }

  int rows, cols;
  fscanf(fp, "%d %d", &rows, &cols);

  int m[30][30];

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      fscanf(fp, "%d", &m[i][j]);
    }
    fscanf(fp, "\n");
  }
  print_matrix(m, rows, cols);

  while (1) {
    char selector[100] = {0};
    int index;
    char direction[100] = {0};
    int position;
    printf("Inserire input: ");
    if (scanf("%s %d %s %d", selector, &index, direction, &position) != 4) {
      printf("Input nel formato sbagliato\n");
      continue;
    }

    if (strcmp(selector, "riga") == 0) {
      printf("%s %d %s %d\n", selector, index, direction, position);

      print_matrix(m, rows, cols);

      if (strcmp(direction, "destra") == 0) {
        shift_matrix_row(m, rows, cols, index - 1, RIGHT, position);
      } else if (strcmp(direction, "sinistra") == 0) {
        shift_matrix_row(m, rows, cols, index - 1, LEFT, position);
      } else {
        printf("Exiting, Unknown direction\n");
        break;
      }

      puts("Shifted");
      print_matrix(m, rows, cols);

      continue;
    }
    if (strcmp(selector, "colonna") == 0) {
      printf("%s %d %s %d\n", selector, index, direction, position);

      print_matrix(m, rows, cols);

      if (strcmp(direction, "su") == 0) {
        shift_matrix_col(m, rows, cols, index - 1, UP, position);
      } else if (strcmp(direction, "giu") == 0) {
        shift_matrix_col(m, rows, cols, index - 1, DOWN, position);
      } else {
        printf("Exiting, Unknown direction\n");
        break;
      }

      puts("Shifted");
      print_matrix(m, rows, cols);
      continue;
    }
    if (strcmp(selector, "fine") == 0) {
      printf("Exiting\n");
      break;
    } else {
      printf("Unknown command\n");
      break;
    }
  }

  fclose(fp);
  return 0;
}