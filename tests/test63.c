#include <stdio.h>

int x1 = 10 * 10 * 10;

static long x2 = (1 << 31) - 1;

int c = 'Z' - 'A';

int main() {
  printf("%d\n", x1);
  printf("%ld\n", x2);
  printf("%c\n", c + 'A');
}
