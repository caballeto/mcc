#include <stdio.h>

int param8(int a, int b, int c, int d, int e, int f, int g, int h) {
  printf("%d\n", a); printf("%d\n", b); printf("%d\n", c); printf("%d\n", d);
  printf("%d\n", e); printf("%d\n", f); printf("%d\n", g); printf("%d\n", h);
  return 0;
}

int param5(int a, int b, int c, int d, int e) {
  printf("%d\n", a);
  printf("%d\n", b);
  printf("%d\n", c);
  printf("%d\n", d);
  printf("%d\n", e);
  return 0;
}

int param2(int a, int b) {
  int c = 3, d = 4, e = 5;
  printf("%d\n", a);
  printf("%d\n", b);
  printf("%d\n", c);
  printf("%d\n", d);
  printf("%d\n", e);
  return 0;
}

int param0() {
  int a = 1, b = 2, c = 3, d = 4, e = 5;
  printf("%d\n", a);
  printf("%d\n", b);
  printf("%d\n", c);
  printf("%d\n", d);
  printf("%d\n", e);
  return 0;
}

int main() {
  param8(1,2,3,4,5,6,7,8);
  printf("%d\n", 111111);
  param5(1,2,3,4,5);
  printf("%d\n", 111111);
  param2(1,2);
  printf("%d\n", 111111);
  param0();
  return(0);
}