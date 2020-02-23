#include <stdio.h>

struct Book {
  int length;
  char *title;
  char *author;
};

typedef struct Book tBook;

union Box {
  char* name;
  long value;
  char c;
  int k;
};

typedef union Box tBox;

int main() {
  tBook book;

  book.length = 25;
  book.title = "The autumn leaves\n";
  book.author = "Frank Sinatra\n";

  printf("%s", book.title);
  printf("%s", book.author);
  printf("%d\n", book.length);

  tBox box;

  box.name = "Box's name\n";
  printf("%s", box.name);

  box.value = 1000;
  printf("%d\n", box.value);

  return 0;
}