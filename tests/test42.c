#include <stdio.h>

struct Book {
  char c;
  int length;
  char* author;
} book;

int main() {
  struct Book* ptr = &book;

  ptr->c = 'a';
  ptr->length = 276;
  ptr->author = "George Orwell 1984\n";

  printf("%ld\n", ptr->c);
  printf("%ld\n", ptr->length);
  printf("%s", ptr->author);

  printf("%ld\n", (*ptr).c);
  printf("%ld\n", (*ptr).length);
  printf("%s", (*ptr).author);

  printf("%ld\n", book.c);
  printf("%ld\n", book.length);
  printf("%s", book.author);

  return 0;
}