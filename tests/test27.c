#include <stdio.h>

int a;
int b[25];

int main() {
  b[3] = 12;
  a = b[3];
  printf("%d\n", a);
  printf("%d\n", b[3]);
  return 0;
}