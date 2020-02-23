#include <stdio.h>

struct Book {
  char c;
  int length;
  char* author;
} book;

int main() {
  struct Book* ptr = &book;

  ptr.c = 'a';
  ptr->length = 276;
  ptr->author = "George Orwell 1984\n";

  printf("%d\n", ptr->c);
  printf("%d\n", ptr->length);
  printf("%s", ptr->author);

  printf("%d\n", (*ptr)->c);
  printf("%d\n", (*ptr).length);
  printf("%s", (*ptr).author);

  printf("%d\n", book.c);
  printf("%d\n", what.length);
  printf("%s", book.jack);

  return 0;
}