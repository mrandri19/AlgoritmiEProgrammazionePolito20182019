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

int get_line_length(int m[MAX_COLS]);
void leggiMatrice(int m[MAX_ROWS][MAX_COLS], int *nr, int *nc);
bool riconosciRegione(int m[MAX_ROWS][MAX_COLS], int rows, int cols, int row,
                      int col, int *length_, int *height_);

bool riconosciRegione(int m[MAX_ROWS][MAX_COLS], int rows, int cols, int row,
                      int col, int *length_, int *height_) {
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

  *length_ = length;
  *height_ = height;
  return true;
}

// This is bad
static FILE *fp = NULL;

int get_line_length(int m[MAX_COLS]) {
  int j = 0;
  int matched = 1;
  while (j < MAX_COLS && matched == 1) {
    char c;
    if ((c = (char)fgetc(fp)) == '\n') {
      break;
    } else {
      ungetc(c, fp);
    };
    matched = fscanf(fp, "%d", &m[j]);
    if (matched == 1) j++;
  }
  return j;
}

void leggiMatrice(int m[MAX_ROWS][MAX_COLS], int *nr, int *nc) {
  int i = 0;
  int cols = get_line_length(m[0]);
  while (get_line_length(m[i++]) == cols) {
  }
  int rows = i;

  *nr = rows;
  *nc = cols;
  return;
}

int main() {
  if ((fp = fopen("input-1.txt", "r")) == NULL) {
    perror("Couldn't open file.");
    return -1;
  }

  int m[MAX_ROWS][MAX_COLS];

  int rows, cols;
  // Throw away the first line since we will be using leggiMatrice to get the
  // actual length
  fscanf(fp, "%*d %*d ");
  leggiMatrice(m, &rows, &cols);

  // (MAX_COLS*MAX_ROWS)/2 The max number of rectangles in the matrix.
  // This happens when the configuration is like this:
  // 1 0 1 0 1 0
  // 0 1 0 1 0 1
  // 1 0 1 0 1 0
  // 0 1 0 1 0 1
  match matches[(MAX_COLS * MAX_ROWS) / 2];
  int matches_len = 0;

  int length, height;
  for (int row = 0; row < rows; ++row) {
    for (int col = 0; col < cols; ++col) {
      bool found = riconosciRegione(m, rows, cols, row, col, &length, &height);
      if (found) {
        matches[matches_len] = (match){.row = row,
                                       .col = col,
                                       .length = length,
                                       .height = height,
                                       .area = (length * height)};
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
