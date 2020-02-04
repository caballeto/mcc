#include <stdio.h>

int main() {
  int x, y;
  short a, b;
  long g, f;
  char *p1, *p2;
  char **pp1, **pp2;
  long **l1, **l2;

  printf("%d", p1 + x);
  printf("%d", p1 - x);
  printf("%d", p1 * x);
  printf("%d", p1 / x);
  printf("%d", a + p1);
  printf("%d", g + p1);
  printf("%d", x + y);
  printf("%d", a + b);
  printf("%d", g + f);
  printf("%d", p1 + p2);
  printf("%d", p1 - p2);
  printf("%d", p1 * p2);
  printf("%d", pp1 + pp2);
  printf("%d", pp1 - pp2);
  printf("%d", pp1 * pp2);
  printf("%d", l1 + l2);
  printf("%d", l1 - l2);
  printf("%d", l1 * l2);
  printf("%d", x + a);
  printf("%d", x + g);
  printf("%d", a + g);
  printf("%d", p1 + pp1);
  printf("%d", p1 - pp1);
  printf("%d", p1 * pp1);
  printf("%d", l1 + p1);
  printf("%d", l1 - p1);
  printf("%d", l1 * p1);
  printf("%d", l1 + pp1);
  printf("%d", l1 - pp1);
  printf("%d", l1 * pp1);
  return 0;
}
