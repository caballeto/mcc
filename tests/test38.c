#include <stdio.h>

int main() {
  int array[20];
  array[0] = 5;
  printf("%d\n", array[0]);
  array[0]++;
  printf("%d\n", array[0]);
  array[0]--;
  printf("%d\n", array[0]);

  int z = array[0];
  printf("%d\n", z);
}