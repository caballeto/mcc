#include <stdio.h>

int main() {
  int x = 25;
  char* c = (char*) &x;
  *c = 'a';
  printf("%c\n", *c);
  return 0;
}