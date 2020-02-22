#include <stdio.h>

void printint(long x) {
  printf("%ld\n", x);
}

void printchar(long x) {
  putc((char)(x & 0x7f), stdout);
}

void printstr(char* str) {
  for (; *str != 0; str++) {
    printchar(*str);
  }
}