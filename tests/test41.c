void printint(long x);
void printchar(long x);

void printstr(char* str) {
  for (; *str != 0; str++) {
    printchar(*str);
  }
}

struct Book {
  char c;
  int length;
  char* author;
} s1;

int main() {
  struct Book s2;

  s1.c = 'a';
  s1.length = 276;
  s1.author = "George Orwell 1984\n";

  printint(s1.c);
  printint(s1.length);
  printstr(s1.author);

  s2.c = 'b';
  s2.length = 1000;
  s2.author = "John Ronald Ruel Tolkien The Lord of the Rings\n";

  printint(s2.c);
  printint(s2.length);
  printstr(s2.author);

  return 0;
}