#include <stdio.h>

int main() {
  int *x, y;
  x = &y;

  long z = x;

  printf("%d\n", x == z);
  printf("%d\n", x + 2 == z + 8);
  printf("%d\n", x - 2 == z - 8);

  x++;
  z = z + 4;

  printf("%d\n", x == z);

  x--;
  z = z - 4;

  printf("%d\n", x == z);

  return 0;
}