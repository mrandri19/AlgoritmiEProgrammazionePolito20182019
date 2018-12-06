#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  char color[2];
  int value[2];
} tile;

tile rotate_tile(tile non_rotated) {
  tile t;

  t.value[0] = non_rotated.value[1];
  t.color[0] = non_rotated.color[1];

  t.value[1] = non_rotated.value[0];
  t.color[1] = non_rotated.color[0];

  return t;
}

typedef struct {
  int id;
  int rotated;
} cell;

typedef cell **board_t;

static int max_score = 0;
static tile **max_tb;

void print_tiles(tile *tiles, size_t tiles_len) {
  for (size_t i = 0; i < tiles_len; i++) {
    printf("%c %d %c %d\n", (tiles[i].color[0]), (tiles[i].value[0]),
           (tiles[i].color[1]), (tiles[i].value[1]));
  }
}

void print_board(board_t board, size_t rows, size_t cols) {
  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < cols; j++) {
      printf("%d/%d ", board[i][j].id, board[i][j].rotated);
    }
    printf("\n");
  }
}

void print_board_with_tiles(tile **tiles, size_t rows, size_t cols) {
  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < cols; j++) {
      tile t = tiles[i][j];
      printf("[%c%d %c%d] ", t.color[0], t.value[0], t.color[1], t.value[1]);
    }
    printf("\n");
  }
}

void check(size_t *sol, size_t k, tile *tiles, size_t n, board_t board,
           size_t rows, size_t cols) {
  // Init board to test the solution on
  tile **tb = calloc(rows, sizeof(tile *));
  for (size_t i = 0; i < rows; i++) {
    tb[i] = calloc(cols, sizeof(tile));
  }

  // Fill the board with tiles
  size_t l = 0;
  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < cols; j++) {
      cell c = board[i][j];
      if (c.id != -1) {
        if (c.rotated == 1) {
          // if it's rotated go forward in the array to pick the rotated version
          // of this tile
          tb[i][j] = tiles[c.id + n / 2];
        } else {
          tb[i][j] = tiles[c.id];
        }
      } else {
        tb[i][j] = tiles[sol[l++]];
      }
    }
  }

  int score = 0;

  // Calculate row score
  for (int i = 0; i < rows; ++i) {
    bool same_color_row = true;
    char first_color_row = tb[i][0].color[0];

    int row_score = 0;
    for (int j = 0; j < cols; ++j) {
      if (tb[i][j].color[0] != first_color_row) {
        same_color_row = false;
      } else {
        row_score += tb[i][j].value[0];
      }
    }
    if (same_color_row) {
      score += row_score;
    }
  }

  // Calculate column score
  for (int j = 0; j < cols; ++j) {
    bool same_color_col = true;
    char first_color_col = tb[0][j].color[1];

    int col_score = 0;
    for (int i = 0; i < rows; ++i) {
      if (tb[i][j].color[1] != first_color_col) {
        same_color_col = false;
      } else {
        col_score += tb[i][j].value[1];
      }
    }

    if (same_color_col) {
      score += col_score;
    }
  }

  if (score > max_score) {
    max_score = score;

    for (int i = 0; i < rows; ++i) {
      for (int j = 0; j < cols; ++j) {
        max_tb[i][j] = tb[i][j];
      }
    }
  }
  // Cleanup
  for (size_t i = 0; i < rows; i++) {
    free(tb[i]);
  }
  free(tb);

  return;
}

void dispositions(size_t pos, size_t k, size_t *sol, bool *mark, tile *tiles,
                  size_t n, board_t board, size_t rows, size_t cols,
                  cell *used_cells, size_t used_cells_len) {
  if (pos == k) {
    check(sol, k, tiles, n, board, rows, cols);
    return;
  }

  for (size_t i = 0; i < n; i++) {
    bool skip = false;
    for (int j = 0; j < used_cells_len; ++j) {
      if (i == used_cells[j].id) skip = true;
    }
    if (skip) continue;

    // If neither the cell, neither its rotated version are used
    if (!mark[i] && !mark[i + n / 2]) {
      // mark both
      mark[i] = true;
      mark[i + n / 2] = true;

      sol[pos] = i;
      dispositions(pos + 1, k, sol, mark, tiles, n, board, rows, cols,
                   used_cells, used_cells_len);

      // unmark both
      mark[i] = false;
      mark[i + n / 2] = false;
    }
  }
}

void solve(tile *tiles, size_t tiles_len, int free_cells, board_t board,
           size_t rows, size_t cols, cell *used_cells, size_t used_cells_len) {
  // Init solution array
  size_t sol_len = free_cells;
  size_t *sol = calloc(sol_len, sizeof(size_t));

  // Init mark array used to make the simple dispositions of n in k
  bool *mark = calloc(sol_len, sizeof(bool));
  for (size_t i = 0; i < sol_len; i++) {
    mark[i] = false;
  }

  size_t n = tiles_len;
  size_t k = free_cells;

  dispositions(0, k, sol, mark, tiles, n, board, rows, cols, used_cells,
               used_cells_len);
}

int main() {
  FILE *fp_tiles = fopen("tiles.txt", "r");
  if (fp_tiles == NULL) {
    perror("cannot open tiles");
    exit(EXIT_FAILURE);
  }
  FILE *fp_board = fopen("board.txt", "r");
  if (fp_board == NULL) {
    perror("cannot open board");
    exit(EXIT_FAILURE);
  }

  // Read the tiles and also create the rotated version
  size_t tiles_len = 0;
  fscanf(fp_tiles, "%zu ", &tiles_len);

  // Double the tiles to have a handy reference to a rotated tile because
  // in the solution we will have to try rotated tiles too
  tiles_len *= 2;
  tile *tiles = calloc(tiles_len, sizeof(tile));

  for (size_t i = 0; i < (tiles_len / 2); i++) {
    tile t;
    fscanf(fp_tiles, "%c %d %c %d ", &(t.color[0]), &(t.value[0]),
           &(t.color[1]), &(t.value[1]));
    tiles[i] = t;
  }
  for (size_t i = tiles_len / 2; i < tiles_len; i++) {
    tile non_rotated = tiles[i - tiles_len / 2];
    tiles[i] = rotate_tile(non_rotated);
  }

  // Read the board
  unsigned int rows, cols;
  fscanf(fp_board, "%d %d ", &rows, &cols);

  board_t board = calloc(rows, sizeof(cell *));
  for (size_t i = 0; i < rows; i++) {
    board[i] = calloc(cols, sizeof(cell));
  }

  cell *used_cells = calloc(tiles_len / 2, sizeof(cell));
  size_t used_cells_len = 0;

  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < cols; j++) {
      cell c;
      fscanf(fp_board, "%d/%d ", &c.id, &c.rotated);

      if (c.id != -1) {
        used_cells[used_cells_len++] = c;
      }

      board[i][j] = c;
    }
  }

  // Count the free cells
  int free_cells = 0;
  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < cols; j++) {
      if (board[i][j].id == -1) {
        free_cells++;
      }
    }
  }

  // TODO: free
  max_tb = calloc(rows, sizeof(tile *));
  for (size_t i = 0; i < rows; i++) {
    max_tb[i] = calloc(cols, sizeof(tile));
  }

  // Solve by trying all tiles permutations in the free cells
  solve(tiles, tiles_len, free_cells, board, rows, cols, used_cells,
        used_cells_len);
  printf("max_score %d\n", max_score);
  print_board_with_tiles(max_tb, rows, cols);

  // Cleanup
  free(used_cells);
  for (size_t i = 0; i < rows; i++) {
    free(max_tb[i]);
  }
  free(max_tb);
  for (size_t i = 0; i < rows; i++) {
    free(board[i]);
  }
  free(board);
  free(tiles);
  fclose(fp_board);
  fclose(fp_tiles);

  return 0;
}
