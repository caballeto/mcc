void printint(long x);
void printchar(long x);
void printstr(char* str);

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

  printstr(book.title);
  for (int i = 0; i < 3; i++) {
    printstr(book.authors[i]);
  }

  return 0;
}