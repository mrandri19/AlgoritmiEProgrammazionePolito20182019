#include <stdio.h>
#include <stdlib.h>

typedef int pair_t[2];

int max(int a, int b) { return ((a > b) ? a : b); }

int duration(pair_t p) { return (p[1] - p[0]); }

int opt(int i, pair_t S[]) {
  printf("opt(%d)\n", i);

  if (i == 0) {
    return duration(S[i]);
  }

  // Not selecting it
  int best = opt(i - 1, S);

  for (size_t n = 0; n < i; n++) {
    // compatibility check

    // Se l'inizio dell'i-esimo elemento e' prima della fine dell'n-esimo
    // elemento
    if (S[i][0] < S[n][1])
      continue;

    best = max(best, duration(S[i]) + opt(n, S));
  }

  return best;
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

  for (size_t i = 0; i < S_len; i++) {
    printf("%d %d\n", S[i][0], S[i][1]);
  }

  printf("opt(0) = %d\n", opt(0, S));
  printf("opt(%d) = %d\n", S_len, opt(S_len - 1, S));

  return 0;
}
