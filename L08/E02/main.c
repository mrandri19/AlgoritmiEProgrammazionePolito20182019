#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  char color[2];
  int value[2];
} tile;

typedef struct {
  int id;
  int rotated;
} cell;

typedef cell **board_t;

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

void print_board_with_tiles(board_t board, size_t rows, size_t cols,
                            tile *tiles) {
  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < cols; j++) {
      int id = board[i][j].id;
      if (id == -1) {
        printf("[** **] ");
      } else {
        tile t = tiles[id];
        int rotated = board[i][j].rotated;
        if (rotated == 1) {
          printf("[%c%d %c%d] ", t.color[1], t.value[1], t.color[0],
                 t.value[0]);
        } else {
          printf("[%c%d %c%d] ", t.color[0], t.value[0], t.color[1],
                 t.value[1]);
        }
      }
    }
    printf("\n");
  }
}

void check(size_t *sol, size_t sol_len, tile *tiles, size_t tiles_len,
           board_t board, size_t rows, size_t cols) {

  board_t new_board = calloc(rows, sizeof(cell *));
  for (size_t i = 0; i < rows; i++) {
    new_board[i] = calloc(cols, sizeof(cell));
  }

  size_t k = 0;
  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < cols; j++) {
      cell c = board[i][j];
      if (c.id == -1) {
        c.id = (int)sol[k++];

        // Not rotated if [0, tiles_len), rotated if [tiles_len, 2*tiles_len)
        c.rotated = (k >= tiles_len);
      }
      new_board[i][j] = c;
    }
  }

  // TODO: finish
  printf("a\n");

  // Check rows color
  for (size_t i = 0; i < rows; i++) {
    int id_first = new_board[i][0].id;
    char color_first = tiles[id_first].color[0];

    for (size_t j = 0; j < cols; j++) {
      int id = new_board[i][j].id;

      if (tiles[id].color[0] != color_first) {
        return;
      }
    }
  }

  printf("\nThe *after* board\n");
  print_board_with_tiles(new_board, rows, cols, tiles);

  // Check cols color

  // Cleanup
  for (size_t i = 0; i < rows; i++) {
    free(new_board[i]);
  }
  free(new_board);

  // TODO: finish
  // TODO: implement max value find
  exit(EXIT_SUCCESS);
  return;
}

void dispositions(size_t pos, size_t sol_len, size_t *sol, bool *mark,
                  tile *tiles, size_t tiles_len, board_t board, size_t rows,
                  size_t cols) {
  if (pos == sol_len) {
    check(sol, sol_len, tiles, tiles_len, board, rows, cols);
    return;
  }

  for (size_t i = 0; i < tiles_len; i++) {
    if (!mark[i]) {
      mark[i] = true;
      sol[pos] = i;
      dispositions(pos + 1, sol_len, sol, mark, tiles, tiles_len, board, rows,
                   cols);
      mark[i] = false;
    }
  }
}

void solve(tile *tiles, size_t tiles_len, int free_cells, board_t board,
           size_t rows, size_t cols) {
  // try all possible dispositions of tiles in the free cells
  // assuming a tile can be rotated
  // and find the disposition which yields the max score

  size_t sol_len = 5;
  size_t *sol = calloc(sol_len, sizeof(size_t));
  bool *mark = calloc(sol_len, sizeof(bool));

  for (size_t i = 0; i < sol_len; i++) {
    mark[i] = false;
  }

  // Dispositions of all n tiles in the k free cells
  size_t n = 2 * tiles_len;

  // FIXME: not considering rotated tiles
  size_t k = free_cells;

  dispositions(0, k, sol, mark, tiles, n, board, rows, cols);
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

  size_t tiles_len = 0;
  fscanf(fp_tiles, "%zu ", &tiles_len);

  tile *tiles = calloc(tiles_len, sizeof(tile));

  for (size_t i = 0; i < tiles_len; i++) {
    tile t;
    fscanf(fp_tiles, "%c %d %c %d ", &(t.color[0]), &(t.value[0]),
           &(t.color[1]), &(t.value[1]));
    tiles[i] = t;
  }

  unsigned int rows, cols;
  fscanf(fp_board, "%d %d ", &rows, &cols);
  board_t board = calloc(rows, sizeof(cell *));

  for (size_t i = 0; i < rows; i++) {
    board[i] = calloc(cols, sizeof(cell));
  }

  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < cols; j++) {
      cell c;
      fscanf(fp_board, "%d/%d ", &c.id, &c.rotated);
      board[i][j] = c;
    }
  }

  int free_cells = 0;
  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < cols; j++) {
      if (board[i][j].id == -1) {
        free_cells++;
      }
    }
  }
  solve(tiles, tiles_len, free_cells, board, rows, cols);

  // Cleanup
  for (size_t i = 0; i < rows; i++) {
    free(board[i]);
  }
  free(board);
  free(tiles);
  fclose(fp_board);
  fclose(fp_tiles);

  return 0;
}
