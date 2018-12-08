#include <stdio.h>
#include <stdlib.h>

int max(int a, int b) { return ((a > b) ? a : b); }

int duration(int S[][2], int S_len, int t) {
  int best = 0;
  for (int i = 0; i < t; ++i) {
    if (S[i][1] == t) {
      best = max(best, S[i][1] - S[i][0]);
    }
  }
  return best;
}

int opt(int S[][2], int S_len, int t) {
  if (t == 0) {
    return 0;
  }

  int best = 0;
  best = max(best, opt(S, S_len, t - 1));

  for (int i = 0; i < t; ++i) {
    // TODO: handle case where 2 events finish at the same time
    best = max(best, opt(S, S_len, i) + duration(S, S_len, t));
  }

  return best;
}

int main(int argc, char const *argv[]) {
  int S[][2] = {{1, 2}, {2, 4}, {2, 5}, {3, 5}, {5, 7}, {6, 8}};
  int S_len = sizeof(S) / sizeof(S[0]);

  // Bubble sort pairs on finish time
  for (int i = 0; i < S_len - 1; ++i) {
    for (int j = 0; j < S_len - i - 1; ++j) {
      if (S[j][1] < S[j + 1][1]) {
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

  // find subset that maximises durations sum
  int max_duration = opt(S, S_len, S[S_len - 1][1]);
  printf("max_duration = %d\n", max_duration);

  return 0;
}