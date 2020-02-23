#include <stdio.h>

static int square(int x) {
  return x * x;
}

static int z;

int k;

int main() {
  k = 0;
  z = square(10);
  printf("%d\n", z);
  printf("%d\n", k);
  return 0;
}