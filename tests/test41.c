#include <stdio.h>

struct Book {
  char c;
  int length;
  char* author;
} s1;

int main() {
  struct Book s2;

  s1.c = 'a';
  s1.length = 276;
  s1.author = "George Orwell 1984\n";

  printf("%d\n", s1.c);
  printf("%d\n", s1.length);
  printf("%s", s1.author);

  s2.c = 'b';
  s2.length = 1000;
  s2.author = "John Ronald Ruel Tolkien The Lord of the Rings\n";

  printf("%d\n", s2.c);
  printf("%d\n", s2.length);
  printf("%s", s2.author);

  return 0;
}