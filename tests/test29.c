#include <stdio.h>

int array[20];

int main() {
  int* ptr = array;
  *ptr = 10;
  *(ptr + 1) = 20;
  printf("%d\n", array[0]);
  printf("%d\n", array[1]);
}
