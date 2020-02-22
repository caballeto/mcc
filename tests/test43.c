void printint(long x);
void printchar(long x);
void printstr(char* str);

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

  printint(ptr->c);
  printint(ptr->length);
  printstr(ptr->author);

  printint((*ptr)->c);
  printint((*ptr).length);
  printstr((*ptr).author);

  printint(book.c);
  printint(what.length);
  printstr(book.jack);

  return 0;
}