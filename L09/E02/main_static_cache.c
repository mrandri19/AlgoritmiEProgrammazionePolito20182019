#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int sapphires, emeralds, rubies, topazes;

static int ****tableS;
static int len = 0;

int solveS(int s, int e, int r, int t);
int solveE(int s, int e, int r, int t);
int solveR(int s, int e, int r, int t);
int solveT(int s, int e, int r, int t);

int max(int a, int b) { return ((a > b) ? a : b); }

int solveS(int s, int e, int r, int t) {
  assert(s >= 0 && e >= 0 && r >= 0 && t >= 0);

  int memoized_result = tableS[s][e][r][t];

  if (memoized_result != -1) {
    assert(memoized_result < len);
  }

  if ((memoized_result != -1)) {
    return memoized_result;
  }

  if (s == 0) {
    return 0;
  }

  int best = 0;
  best = max(best, solveS(s - 1, e, r, t));
  best = max(best, solveR(s - 1, e, r, t));

  int result = 1 + best;

  tableS[s][e][r][t] = result;

  return result;
}

int solveE(int s, int e, int r, int t) {
  assert(s >= 0 && e >= 0 && r >= 0 && t >= 0);
  if (e == 0) {
    return 0;
  }

  int best = 0;
  best = max(best, solveE(s, e - 1, r, t));
  best = max(best, solveT(s, e - 1, r, t));

  return 1 + best;
}

int solveR(int s, int e, int r, int t) {
  assert(s >= 0 && e >= 0 && r >= 0 && t >= 0);
  if (r == 0) {
    return 0;
  }

  int best = 0;
  best = max(best, solveE(s, e, r - 1, t));
  best = max(best, solveT(s, e, r - 1, t));

  return 1 + best;
}

int solveT(int s, int e, int r, int t) {
  assert(s >= 0 && e >= 0 && r >= 0 && t >= 0);
  if (t == 0) {
    return 0;
  }

  int best = 0;
  best = max(best, solveS(s, e, r, t - 1));
  best = max(best, solveR(s, e, r, t - 1));

  return 1 + best;
}

int main() {
  int tests_len;
  fscanf(stdin, "%d", &tests_len);

  for (int i = 0; i < tests_len; i++) {
    fscanf(stdin, "%d %d %d %d ", &sapphires, &rubies, &topazes, &emeralds);

    int total = sapphires + emeralds + rubies + topazes;

    len = (sapphires + 1) + (emeralds + 1) + (rubies + 1) + (topazes + 1);

    // Malloc 4D...
    // I spent 2 hours trying to allocate a contiguos vector and then
    tableS = malloc((sapphires + 1) * sizeof(int ***));
    for (int i = 0; i < (sapphires + 1); i++) {
      tableS[i] = malloc((emeralds + 1) * sizeof(int **));
      for (int j = 0; j < (emeralds + 1); j++) {
        tableS[i][j] = malloc((rubies + 1) * sizeof(int *));
        for (int k = 0; k < (rubies + 1); k++) {
          tableS[i][j][k] = malloc((topazes + 1) * sizeof(int));
          for (int l = 0; l < (topazes + 1); l++) {
            tableS[i][j][k][l] = -1;
          }
        }
      }
    }

    printf("TEST #%d\n", i + 1);
    printf("zaffiro = %d, rubino = %d, topazio = %d, smeraldo = %d, TOT = %d\n",
           sapphires, rubies, topazes, emeralds, total);

    int best = 0;

    best = max(best, solveS(sapphires, emeralds, rubies, topazes));
    best = max(best, solveE(sapphires, emeralds, rubies, topazes));
    best = max(best, solveR(sapphires, emeralds, rubies, topazes));
    best = max(best, solveT(sapphires, emeralds, rubies, topazes));

    printf("Collana massima di lunghezza %d\n", best);
  }

  return 0;
}
