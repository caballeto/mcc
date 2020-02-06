#include <stdio.h>
#include <stdlib.h>

int p(int*** c1, int** c2, int* c3) {
  c2 = &c3;
  c1 = &c2;
  ***c1 = 25;
  return ***c1;
}

int main() {
  return 0;
}