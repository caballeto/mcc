#include <stdio.h>

int array[10], z;

int main() {
  int* x = array;
  int** y = array;


  x[1] = 10;

  printf("%d\n", array[1]);

  array[0] = 5;
  array[0]++;

  printf("%d\n", array[0]);

  x = &array[0];
  *x = 10;

  printf("%d\n", array[0]);

  return 0;
}