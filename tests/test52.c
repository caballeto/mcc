void printint(long x);
void printstr(char* str);

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

  printstr(book.title);
  printstr(book.author);
  printint(book.length);

  struct X x;

  x.length = 25;
  x.title = "The autumn leaves\n";
  x.author = "Frank Sinatra\n";

  printstr(x.title);
  printstr(x.author);
  printint(x.length);

  Box box;

  box.name = "Box's name\n";
  printstr(box.name);

  box.value = 1000;
  printint(box.value);

  union Y y;

  y.name = "Box's name\n";
  printstr(y.name);

  y.value = 1000;
  printint(y.value);

  return 0;
}