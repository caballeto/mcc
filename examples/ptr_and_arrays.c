#include <stdio.h>

int array[20];

int main() {
  int* ptr = &array[0];
  *ptr = 10;
  *(ptr + 1) = 20;
  ptr[2] = 30;
  printf("%d\n", array[0]);
  printf("%d\n", array[1]);
  printf("%d\n", array[2]);
}
