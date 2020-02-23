#include <stdio.h>

int main() {
  int x = 0;
  ++x;
  printf("%d\n", x);
  printf("%d\n", ++x);
  printf("%d\n", x);
  return 0;
}