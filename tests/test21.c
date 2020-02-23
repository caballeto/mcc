#include <stdio.h>

int func() {
  int y = 5;
  if (y == 5) {
    return 25;
  } else {
    return 1;
  }
}

int main() {
  int z = func();
  printf("%d\n", z);
  return 0;
}