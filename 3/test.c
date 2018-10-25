#include <limits.h>
#include <stdio.h>
int main() {
  long double decker = 6.75;
  int i;

  printf("sizeof(decker): %luB %lub\n", sizeof(decker),
         sizeof(decker) * CHAR_BIT);
  for (i = 0; i < sizeof(decker); i++) {
    printf("%.2d ", i);
  }

  unsigned char bits[128] = {0};
  printf("\n");
  for (i = 0; i < sizeof(decker); i++) {
    unsigned char byte = *(((unsigned char *)&decker) + i);
    printf("%02X ", byte);
    unsigned long j = 0;
    while (byte > 0) {
      bits[(i * CHAR_BIT) + j] = byte % 2;
      byte /= 2;
      j++;
    }
  }
  printf("\n");
  printf("\n");

  for (int i = 0; i < sizeof(decker) * CHAR_BIT; ++i) {
    printf("%u", bits[i]);
    if (i % 8 == 7) printf(" ");
  }
  printf("\n");

  return 0;
}