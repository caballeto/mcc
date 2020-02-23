#include <stdio.h>

short array[20];

int main() {
  array[0] = 5;
  printf("%i\n", array[0]);
  array[0]++;
  printf("%i\n", array[0]);
  array[0]--;
  printf("%i\n", array[0]);
}
