#include <stdio.h>

enum fred { apple, banana, carrot = 10, pear = 10, peach, mango, papaya };
enum jane { aple, bnana, crrot, par, pech, mago, paaya };

enum fred var1;
enum jane var2;
enum fred var3;

int main() {
  var1 = carrot + pear + mango;
  printf("%d\n", var1);
  return 0;
}