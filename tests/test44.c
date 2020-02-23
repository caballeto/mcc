#include <stdio.h>

struct Book {
  int length;
  char* title;
  char* authors[3];
} book;

int main() {
  book.authors[0] = "Jack Welch\n";
  book.authors[1] = "Max Levchin\n";
  book.authors[2] = "Peter Thiel\n";

  book.length = 100;
  book.title = "Super great book\n";

  printf("%s", book.title);
  for (int i = 0; i < 3; i++) {
    printf("%s", book.authors[i]);
  }

  return 0;
}