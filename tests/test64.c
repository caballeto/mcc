#include <stdio.h>

int main() {
  int x = 0;
  x += 10;
  x += 20;
  x += 20;

  printf("%d\n", x);
  printf("%d\n", x % 7);

  printf("%d\n", x *= 10);

  printf("%d\n", x);

  int* y = &x;

  *y /= 10;

  printf("%d\n", x);

  x %= 7;

  printf("%d\n", x);

  x <<= 8;

  printf("%d\n", x);

  *y >>= 6;

  printf("%d\n", x);

  x = 10;
  x &= 12;
  printf("%d\n", x);

  x = 10;
  x |= 12;
  printf("%d\n", x);

  x = 10;
  x ^= 12;
  printf("%d\n", x);

  return 0;
}
