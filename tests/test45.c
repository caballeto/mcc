#include <stdio.h>

union Book {
  char c;
  int length;
  char* author;
} book;

int main() {
  book.author = "This is it - Union!\n";
  printf("%s", book.author);

  union Book u;
  u.author = "And this is a local union.\n";
  printf("%s", u.author);

  union Book* ptr = &u;
  ptr->author = "And this is a pointer assigned union member.\n";
  printf("%s", ptr->author);

  book.length = 100;
  printf("%d\n", book.length);

  return 0;
}