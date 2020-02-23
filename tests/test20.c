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
  int some_int = 25;
  printf("%d\n", some_int * 10);
  return some_int;
}