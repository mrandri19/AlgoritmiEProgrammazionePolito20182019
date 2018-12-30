#ifndef UTIL_H
#define UTIL_H

int min(int a, int b) {
  if (a > b) {
    return b;
  }
  return a;
}

int max(int a, int b) {
  if (a > b) {
    return a;
  }
  return b;
}

#endif
