void printint(long x);
void printchar(long x);
void printstr(char* str);

union Book {
  char c;
  int length;
  char* author;
} book;

int main() {
  book.author = "This is it - Union!\n";
  printstr(book.author);

  union Book u;
  u.author = "And this is a local union.\n";
  printstr(u.author);

  union Book* ptr = &u;
  ptr->author = "And this is a pointer assigned union member.\n";
  printstr(ptr->author);

  book.length = 100;
  printint(book.length);

  return 0;
}