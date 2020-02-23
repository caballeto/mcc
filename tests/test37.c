#include <stdio.h>
void printint(long x);

long fib(int x) {
  if (x == 0) {
    return x;
  } else if (x == 1) {
    return x;
  } else {
    return fib(x - 1) + fib(x - 2);
  }
}

int main() {
  printint(fib(0));
  printint(fib(1));
  printint(fib(2));
  printint(fib(3));
  printint(fib(4));
  printint(fib(5));
  printint(fib(6));
  printint(fib(7));
  printint(fib(8));
  printint(fib(9));
  printint(fib(40));
}