#include <stdio.h>

int func(int x) {
  if (x == 25) {
    goto result;
  }

  printf("%d\n", x - 25);
  return x - 25;

result:
  printf("%d\n", x);
  return x;
}

int main() {
	func(0);
	func(25);
}