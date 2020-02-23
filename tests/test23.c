#include <stdio.h>

int main() {
  int *x;
  int z = 0;

  x = &z;
  *x = 5;

  printf("%d\n", (*x)--);
  printf("%d\n", *x);
  printf("%d\n", --(*x));
  printf("%d\n", *x);

  return 0;
}