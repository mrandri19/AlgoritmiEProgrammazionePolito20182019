#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void print_arr(int v[], int n) {
  for (int i = 0; i < n; i++) {
    printf("%d\n", v[i]);
  }
}

void swap(int A[], int i, int j) {
  int tmp = A[i];
  A[i] = A[j];
  A[j] = tmp;
}

int partition(int A[], int l, int r) {
  int i = l - 1, j = r;

  while (true) {
    while (A[++i] < A[r]) {
    };
    while (A[--j] > A[r]) {
      if (j == l) {
        break;
      }
    };

    if (i >= j) {
      break;
    }
    swap(A, i, j);
  }
  swap(A, i, r);
  return i;
}

void quicksort_aux(int A[], int l, int r) {
  if (r <= l)
    return;
  int q = partition(A, l, r);
  quicksort_aux(A, l, q - 1);
  quicksort_aux(A, q + 1, r);
}

void quicksort(int A[], int n) {
  int l = 0, r = n - 1;
  quicksort_aux(A, l, r);
}

int majority(int *a, int N) {
  int current_elem = a[0];
  int occurrences = 1;
  for (int i = 1; i < N; ++i) {
    if (a[i] == current_elem) {
      occurrences++;
    } else {
      if (occurrences > (N / 2)) {
        return a[i - 1];
      };
      occurrences = 1;
      current_elem = a[i];
    }
  }
  return -1;
}

int main() {
  int n = 7;
  int v[] = {
      3, 3, 9, 4, 3, 5, 3,
  };

  print_arr(v, n);

  quicksort(v, n);

  printf("\n");
  print_arr(v, n);

  int m = majority(v, n);
  if (m != -1) {
    printf("Majority element found: %d\n", m);
  } else {
    printf("No majority element found\n");
  }

  return 0;
}
