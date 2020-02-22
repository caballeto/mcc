void printint(long x);
void printstr(char* str);

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
  printstr(ptr->title);
  printstr(ptr->author);
  printint(ptr->length);
  return 0;
}
