#include <stdio.h>
#define LEN 30

int max(int a, int b) { return (a > b) ? a : b; }

int main() {
  int v[LEN];
  printf(
      "Insert numbers, spaces/newlines are ignored and everything else stops "
      "the acquisition\n");

  int len = 0;
  while ((scanf("%d", &v[len]) == 1) && len < LEN) {
    len++;
  }

  printf("Length: %d\n", len);
  for (int i = 0; i < len; ++i) {
    printf("%d\n", v[i]);
  }

  int max_cont_len = 0;
  int curr_len = 0;
  for (int i = 0; i < len; ++i) {
    if (v[i] != 0) {
      curr_len++;
      max_cont_len = max(max_cont_len, curr_len);
    } else {
      curr_len = 0;
    }
  }
  printf("Max contiguous len: %d\n", max_cont_len);
  printf("Current sequence len: %d\n", curr_len);

  curr_len = 0;
  int tmp_v[LEN] = {0};
  int tmp_v_pos = 0;
  for (int i = 0; i < len; ++i) {
    if (v[i] != 0) {
      tmp_v[tmp_v_pos] = v[i];
      curr_len++;
      tmp_v_pos++;
    } else {
      curr_len = 0;
      tmp_v_pos = 0;
      // zero the array
      for (int j = 0; j < max_cont_len; ++j) {
        tmp_v[j] = 0;
      }
    }
    if (curr_len == max_cont_len) {
      printf("%s\n", "Printing array");

      for (int j = 0; j < max_cont_len; ++j) {
        printf("%d\n", tmp_v[j]);
      }
    }
  }

  return 0;
}