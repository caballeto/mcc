#include <stdio.h>

int array[20];

int main() {
  array[0] = 5;
  printf("%d\n", array[0]);
  array[0]++;
  printf("%d\n", array[0]);
  array[0]--;
  printf("%d\n", array[0]);
}