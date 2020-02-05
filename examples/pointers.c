#include <stdio.h>
#include <stdlib.h>

int* p(int* c1, long* c2, short* c3, int* x) {
  int* z = &(*x);
  return z;
}

int main() {
  return 0;
}