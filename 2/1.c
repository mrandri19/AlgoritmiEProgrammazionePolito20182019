#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROWS 50
#define MAX_COLS 50

typedef struct {
  int row;
  int col;
  int length;
  int height;
  int area;
} match;

bool search_block(int m[MAX_ROWS][MAX_COLS], int rows, int cols, int row,
                  int col, match matches[(MAX_ROWS * MAX_COLS) / 2],
                  int matches_len);

bool search_block(int m[MAX_ROWS][MAX_COLS], int rows, int cols, int row,
                  int col, match matches[(MAX_ROWS * MAX_COLS) / 2],
                  int matches_len) {
  if (m[row][col] == 0) {
    return false;
  }

  int length = 0, height = 0;

  // Since they can only be rectangles we find the lenght of the first
  // horizontal strip and then go down while the first element is filled with
  // ones.

  // Find the first horizontal strip
  while ((m[row + height][col + length] == 1) && ((col + length) < cols)) {
    length++;
  }
  height++;

  // Since we only have rectangles we can just go down while the first element
  // of the column is one, without checking them all
  // suppose the matrix is this one:
  // 1 1 1 0 0
  // 1 1 1 0 0
  // 1 1 1 0 0
  // 0 0 0 0 0
  // we just need to check this ones:(marked with * if they contain 1 and / if 0
  // )
  // * * * / 0
  // * / 1 0 0
  // * / 1 0 0
  // / 0 0 0 0
  // this will return 3,3
  int l = 0;
  while ((m[row + height][col + l] == 1) && ((row + height) < rows)) {
    height++;
  }

  matches[matches_len] = (match){.row = row,
                                 .col = col,
                                 .length = length,
                                 .height = height,
                                 .area = (length * height)};
  return true;
}

int main() {
  FILE* fp;
  if ((fp = fopen("input-1.txt", "r")) == NULL) {
    perror("Couldn't open file.");
    return -1;
  }

  int m[MAX_ROWS][MAX_COLS];

  int rows, cols;
  fscanf(fp, "%d %d", &rows, &cols);

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      fscanf(fp, "%d", &m[i][j]);
    }
  }

  // (MAX_COLS*MAX_ROWS)/2 The max number of rectangles in the matrix.
  // This happens when the configuration is like this:
  // 1 0 1 0 1 0
  // 0 1 0 1 0 1
  // 1 0 1 0 1 0
  // 0 1 0 1 0 1
  match matches[(MAX_COLS * MAX_ROWS) / 2];
  int matches_len = 0;

  for (int row = 0; row < rows; ++row) {
    for (int col = 0; col < cols; ++col) {
      bool found = search_block(m, rows, cols, row, col, matches, matches_len);
      if (found) {
        matches_len++;
      }
    }
  }

  int max_length_i = 0, max_height_i = 0, max_area_i = 0;
  for (int i = 0; i < matches_len; ++i) {
    if (matches[i].length > matches[max_length_i].length) {
      max_length_i = i;
    }
    if (matches[i].height > matches[max_height_i].height) {
      max_height_i = i;
    }
    if (matches[i].area > matches[max_area_i].area) {
      max_area_i = i;
    }
  }

  printf("Max Base: estr. sup. SX=<%d,%d> b=%d, h=%d, Area=%d\n",
         matches[max_length_i].row, matches[max_length_i].col,
         matches[max_length_i].length, matches[max_length_i].height,
         matches[max_length_i].area);
  printf("Max Area: estr. sup. SX=<%d,%d> b=%d, h=%d, Area=%d\n",
         matches[max_area_i].row, matches[max_area_i].col,
         matches[max_area_i].length, matches[max_area_i].height,
         matches[max_area_i].area);
  printf("Max Altezza: estr. sup. SX=<%d,%d> b=%d, h=%d, Area=%d\n",
         matches[max_height_i].row, matches[max_height_i].col,
         matches[max_height_i].length, matches[max_height_i].height,
         matches[max_height_i].area);

  return 0;
}
