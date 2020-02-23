#include <stdio.h>

typedef struct X {
  int length;
  char *title;
  char *author;
} Book;

Book* set_lotr(Book* book) {
  book->length = 100;
  book->title = "Lord of the Rings\n";
  book->author = "John Tolkien\n";
  return book;
}

int main() {
  struct X book;
  Book* ptr = set_lotr(&book);
  printf("%s", ptr->title);
  printf("%s", ptr->author);
  printf("%d\n", ptr->length);
  return 0;
}
