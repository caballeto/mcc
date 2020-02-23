#include <stdio.h>

typedef int* intptr;
typedef int Int;

Int main() {
  intptr x1, *x2, **x3;
  Int x = 10;
  x1 = &x;
  x2 = &x1;
  x3 = &x2;
  printf("%d\n", ***x3);
  return 0;
}