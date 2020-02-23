#include <stdio.h>

enum {
  peter = 1,
  donald = 2,
  ronald = 3,
  john = 4
} var;

int main() {
  var = peter + donald + ronald + john;
  printf("%d\n", var);
  return 0;
}