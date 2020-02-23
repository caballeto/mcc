#include <stdio.h>

typedef struct X {
  int length;
  char *title;
  char *author;
} Book;

typedef union Y {
  char* name;
  long value;
  char c;
  int k;
} Box;

int main() {
  Book book;

  book.length = 25;
  book.title = "The autumn leaves\n";
  book.author = "Frank Sinatra\n";

  printf("%s", book.title);
  printf("%s", book.author);
  printf("%d\n", book.length);

  struct X x;

  x.length = 25;
  x.title = "The autumn leaves\n";
  x.author = "Frank Sinatra\n";

  printf("%s", x.title);
  printf("%s", x.author);
  printf("%d\n", x.length);

  Box box;

  box.name = "Box's name\n";
  printf("%s", box.name);

  box.value = 1000;
  printf("%d\n", box.value);

  union Y y;

  y.name = "Box's name\n";
  printf("%s", y.name);

  y.value = 1000;
  printf("%d\n", y.value);

  return 0;
}