#include <stdio.h>

void print_vec(int *vec, int len) {
  for (int i = 0; i < len; ++i) {
    if (i == len - 1) {
      printf("%d\n", vec[i]);
    } else {
      printf("%d ", vec[i]);
    }
  }
}
void shift_left(int *vec, int len) { return; }

void shift_right(int *vec, int len) {
  int a = vec[len - 1];
  int b;
  for (int i = 0; i < len; ++i) {
    b = vec[i];
    vec[i] = a;
    a = b;
  }
}

int main() {
  int vec[30];
  int len = 0;

  int n;
  scanf("%d", &n);
  while (1) {
    if (scanf("%d", &vec[len]) != 1) {
      while ((getchar()) != '\n')
        ;
      break;
    }
    len++;

    if (len > 30) {
      break;
    }
  }

  print_vec(vec, len);

  int command;
  while (1) {
    scanf("%d", &command);
    if (command == 1) {
      printf("Left\n");
      for (int i = 0; i < n; ++i) {
        shift_left(vec, len);
      }
      break;
    } else if (command == -1) {
      printf("Right\n");
      for (int i = 0; i < n; ++i) {
        shift_right(vec, len);
      }
      print_vec(vec, len);
      break;
    } else if (command == 0) {
      printf("Bye\n");
      break;
    } else {
      printf("Try again\n");
    }
  }

  return 0;
}