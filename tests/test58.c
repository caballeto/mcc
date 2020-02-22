void printint(long x);

struct Book {
  int length;
  char* ptr;
  char c;
};

union Box {
  int length;
  char* ptr;
  char c;
};

int main() {
  char x1;
  short x2;
  int x3;
  long x4;

  printint(sizeof(x1));
  printint(sizeof(x2));
  printint(sizeof(x3));
  printint(sizeof(x4));

  char* p1;
  short* p2;
  int* p3;
  long* p4;

  printint(sizeof(p1));
  printint(sizeof(p2));
  printint(sizeof(p3));
  printint(sizeof(p4));

  char a1[10];
  short a2[10];
  int a3[10];
  long a4[10];
  char* a5[10];

  printint(sizeof(a1));
  printint(sizeof(a2));
  printint(sizeof(a3));
  printint(sizeof(a4));
  printint(sizeof(a5));

  struct Book b1;
  struct Book* b2;

  printint(sizeof(b1));
  printint(sizeof(b2));

  union Box c1;
  union Box* c2;

  printint(sizeof(c1));
  printint(sizeof(c2));

  struct Book books[10];
  union Box boxes[10];

  printint(sizeof(books));
  printint(sizeof(boxes));

  printint(100000);

  printint(sizeof(char));
  printint(sizeof(short));
  printint(sizeof(int));
  printint(sizeof(long));
  printint(sizeof(char*));
  printint(sizeof(struct Book));
  printint(sizeof(struct Book*));
  printint(sizeof(union Box));
  printint(sizeof(union Box*));
}
