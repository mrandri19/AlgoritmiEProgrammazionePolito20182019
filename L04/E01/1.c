#include <stdio.h>
#include <stdlib.h>

int gcd(int a, int b) {
  printf("gcd(%d,%d)\n", a, b);

  if (a == 0)
    return b;
  if (b == 0)
    return a;

  int tmp;
  if (a < b) {
    tmp = a;
    a = b;
    b = tmp;
  }

  if ((a % 2 == 0) && (b % 2 == 0)) {
    return 2 * gcd(a / 2, b / 2);
  } else if ((a % 2 == 1) && (b % 2 == 0)) {
    return gcd(a, b / 2);
  } else if ((a % 2 == 1) && (b % 2 == 1)) {
    return gcd((a - b) / 2, b);
  } else {
    printf("a: %d, b: %d\n", a, b);
    perror("Impossible");
    exit(-1);
  }
}

int main() {
  int a = 48, b = 360;

  printf("%d\n", gcd(a, b));

  return 0;
}