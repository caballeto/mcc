#include <stdio.h>

int main() {
  char z; int y; int x;
  x = 10;  y = 20; z = 30;

  if (x == 10) {
    int x = 100, y = 200, z = 300;
    printf("%d\n", x);
    printf("%d\n", y);
    printf("%d\n", z);
  }

  printf("%d\n", x);
  printf("%d\n", y);
  printf("%d\n", z);

  int main = 25;

  return 0;
}