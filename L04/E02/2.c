#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int count_occurrences(int *v, int l, int r, int element) {
  int counter = 0;
  for (int i = l; i <= r; ++i) {
    if (element == v[i]) counter++;
  }
  return counter;
}

int majority_rec(int *v, int l, int r) {
  int len = r - l + 1;
  int mid = (r + l) / 2;
  if (len == 1) {
    return v[l];
  } else if (len == 2) {
    if (v[l] == v[r]) {
      return v[l];
    } else {
      return -1;
    }
  } else {
    int out1 = majority_rec(v, l, mid);
    int out2 = majority_rec(v, mid + 1, r);

    if (out1 == -1 && out2 == -1) {
      return -1;
    }
    if (out1 != -1 && out2 == -1) {
      // count occurrences of out1
      int occ = count_occurrences(v, l, r, out1);
      if (occ > len / 2) {
        return out1;
      } else {
        return -1;
      }
    }
    if (out1 == -1 && out2 != -1) {
      // count occurrences of out2
      int occ = count_occurrences(v, l, r, out2);
      if (occ > len / 2) {
        return out2;
      } else {
        return -1;
      }
    }
    if (out1 != -1 && out2 != -1) {
      // count occurrences of out1 and out2 and pick the best
      int occ1 = count_occurrences(v, l, r, out1);
      int occ2 = count_occurrences(v, l, r, out2);
      if (occ1 > len / 2) {
        return out1;
      } else if (occ2 > len / 2) {
        return out2;
      } else {
        return -1;
      }
    }

    printf("Impossible!\n");
    return -1;
  }
}

int majority(int *v, int n) { return majority_rec(v, 0, n - 1); }

int main() {
  int v[] = {3, 3, 9, 4, 3, 5, 3};
  int n = sizeof(v) / sizeof(v[0]);

  int m = majority(v, n);
  if (m != -1) {
    printf("Majority element found: %d\n", m);
  } else {
    printf("No majority element found\n");
  }

  return 0;
}
