#include <stdio.h>

struct S {
  int y;
  int k;
};

int main() {
  const char* str = "abc";

  if (str) {
    printf(str);
  }

  int **p;

  if (p) {
    printf("Any pointer can be in result");
  }

  int m[4] = {1, 2, 3, 4};

  if (m) {
    printf("Arrays could be also.");
  }

  struct S s;

  if (s) {
    printf("Structs also");
  }

  return 0;
}