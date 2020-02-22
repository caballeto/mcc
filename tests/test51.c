void printint(long x);
void printstr(char* str);

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

  printstr(book.title);
  printstr(book.author);
  printint(book.length);

  tBox box;

  box.name = "Box's name\n";
  printstr(box.name);

  box.value = 1000;
  printint(box.value);

  return 0;
}