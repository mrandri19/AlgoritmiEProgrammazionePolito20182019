#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static int *tableS;
static int *tableE;
static int *tableR;
static int *tableT;

static int sapphires, emeralds, rubies, topazes;
static int len = 0;

int solveS(int s, int e, int r, int t);
int solveE(int s, int e, int r, int t);
int solveR(int s, int e, int r, int t);
int solveT(int s, int e, int r, int t);

int max(int a, int b) { return ((a > b) ? a : b); }

int solveS(int s, int e, int r, int t) {
  assert(s >= 0 && e >= 0 && r >= 0 && t >= 0);

  // TODO: fucking finish this
  int offset = (s * (emeralds + 1) * (rubies + 1) * topazes) +
               (e * (rubies + 1) * (topazes + 1)) + (r * (topazes + 1)) + t;

  if (offset >= len) {
    printf("len = %d\n", len);
    printf("offset = %d\n", offset);
    printf("%d %d %d %d\n", s, e, r, t);
    assert(false);
  }

  int memoized_result = tableS[offset];
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

  tableS[offset] = result;

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

    printf("TEST #%d\n", i + 1);
    printf("zaffiro = %d, rubino = %d, topazio = %d, smeraldo = %d, TOT = %d\n",
           sapphires, rubies, topazes, emeralds, total);

    int best = 0;

    len = (sapphires + 1) * (emeralds + 1) * (rubies + 1) * (topazes + 1);
    tableS = malloc(len * sizeof(int));
    tableE = malloc(len * sizeof(int));
    tableR = malloc(len * sizeof(int));
    tableT = malloc(len * sizeof(int));

    for (size_t i = 0; i < len; i++) {
      tableS[i] = -1;
    }

    assert(tableS != NULL);
    assert(tableE != NULL);
    assert(tableR != NULL);
    assert(tableT != NULL);

    best = max(best, solveS(sapphires, emeralds, rubies, topazes));
    best = max(best, solveE(sapphires, emeralds, rubies, topazes));
    best = max(best, solveR(sapphires, emeralds, rubies, topazes));
    best = max(best, solveT(sapphires, emeralds, rubies, topazes));

    printf("Collana massima di lunghezza %d\n", best);

    free(tableS);
    free(tableE);
    free(tableR);
    free(tableT);
  }

  return 0;
}
