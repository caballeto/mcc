#include <stdio.h>


enum jack { apple=1, banana, carrot, pear, peach, mango=5, papaya=5 };
enum jack x = 25;

int main() {
  int m = 10;
  int x = apple;
  apple = 5;
  printf("%d\n", mango);
  printf("%d\n", peach);
  printf("%d\n", papaya);
  return (0);
}