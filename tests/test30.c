#include <stdio.h>

int array[20];

int main() {
  int* ptr = &array[0];
  *ptr = 10;
  *(ptr + 1) = 20;
  ptr[2] = 30;
  *(array + 3) = 40;

  printf("%d\n", array[0]);
  printf("%d\n", array[1]);
  printf("%d\n", array[2]);
  printf("%d\n", array[3]);

  printf("%d\n", ptr[0]);
  printf("%d\n", *ptr);
  printf("%d\n", ptr[1]);
  printf("%d\n", *(ptr + 1));
  printf("%d\n", ptr[2]);
  printf("%d\n", *(ptr + 2));
  printf("%d\n", ptr[3]);
  printf("%d\n", *(ptr + 3));
}
