#include <stdio.h>
#include <stdlib.h>

typedef int pair_t[2];

int max(int a, int b) { return ((a > b) ? a : b); }

int duration(pair_t p) { return (p[1] - p[0]); }

int opt(int i, pair_t S[], int S_len) {
  int *solution = calloc(S_len, sizeof(int));

  // Print start array
  for (int n = 0; n < S_len; n++) {
    printf("%d ", n);
  }
  printf("\n");
  for (int n = 0; n < S_len; n++) {
    printf("%d ", solution[n]);
  }
  printf("\n");

  // Solve
  solution[0] = duration(S[0]);
  printf("\n");

  for (int n = 1; n <= i; n++) {
    int best = solution[n - 1];

    for (int j = 0; j < n; j++) {
      if (S[n][0] < S[j][1]) {
        continue;
      }
      best = max(best, duration(S[n]) + solution[j]);
    }

    solution[n] = best;
  }

  // Print end array
  for (int n = 0; n < S_len; n++) {
    printf("%d ", n);
  }
  printf("\n");
  for (int n = 0; n < S_len; n++) {
    printf("%d ", solution[n]);
  }
  printf("\n");

  free(solution);
  return solution[S_len - 1];
}

int main() {
  pair_t S[] = {{1, 2}, {2, 4}, {2, 5}, {3, 5}, {5, 7}, {6, 8}};
  int S_len = sizeof(S) / sizeof(S[0]);

  // Bubble sort pairs on finish time
  for (int i = 0; i < S_len - 1; ++i) {
    for (int j = 0; j < S_len - i - 1; ++j) {
      if (S[j][1] > S[j + 1][1]) {
        int tmp[2];
        tmp[0] = S[j][0];
        tmp[1] = S[j][1];

        S[j][0] = S[j + 1][0];
        S[j][1] = S[j + 1][1];

        S[j + 1][0] = tmp[0];
        S[j + 1][1] = tmp[1];
      }
    }
  }

  for (int i = 0; i < S_len; i++) {
    printf("%d %d\n", S[i][0], S[i][1]);
  }

  printf("opt(%d) = %d\n", S_len, opt(S_len - 1, S, S_len));

  return 0;
}
