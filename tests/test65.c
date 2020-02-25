#include <stdio.h>

int a = 1;

int main() {
  int z;
  for (int x = 0; x <= 1; x++) {
    for (int y = 0; y <= 1; y++) {
      z = x && y;
      printf("%d %d | %d\n", x, y, z);
    }
  }

  for (int x = 0; x <= 1; x++) {
    for (int y = 0; y <= 1; y++) {
      z = x || y;
      printf("%d %d | %d\n", x, y, z);
    }
  }

  // Now some lazy evaluation
  int* ptr = (int*)NULL;
  if (ptr && *ptr == 1)
    printf("ptr points at 1\n");
  else
    printf("ptr is NULL or doesn't point at 1\n");

  ptr = &a;
  if (ptr && *ptr == 1)
    printf("ptr points at 1\n");
  else
    printf("ptr is NULL or doesn't point at 1\n");

  return 0;
}