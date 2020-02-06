#include <stdio.h>

int y, m = 5;

int x = 5;

int func(int x) {
  if (x == 5) {
    return x * 10;
  } else {
    int y = 25;
    x += y;
    return y - x;
  }
  x++;
  x -= 10;
}

int main() {
  int x = 5;
  char c = 5;
  int *y = &x;
  int **z = &y;
  return 0;
}