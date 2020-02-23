#include <stdio.h>

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

  printf("%d\n", sizeof(x1));
  printf("%d\n", sizeof(x2));
  printf("%d\n", sizeof(x3));
  printf("%d\n", sizeof(x4));

  char* p1;
  short* p2;
  int* p3;
  long* p4;

  printf("%d\n", sizeof(p1));
  printf("%d\n", sizeof(p2));
  printf("%d\n", sizeof(p3));
  printf("%d\n", sizeof(p4));

  char a1[10];
  short a2[10];
  int a3[10];
  long a4[10];
  char* a5[10];

  printf("%d\n", sizeof(a1));
  printf("%d\n", sizeof(a2));
  printf("%d\n", sizeof(a3));
  printf("%d\n", sizeof(a4));
  printf("%d\n", sizeof(a5));

  struct Book b1;
  struct Book* b2;

  printf("%d\n", sizeof(b1));
  printf("%d\n", sizeof(b2));

  union Box c1;
  union Box* c2;

  printf("%d\n", sizeof(c1));
  printf("%d\n", sizeof(c2));

  struct Book books[10];
  union Box boxes[10];

  printf("%d\n", sizeof(books));
  printf("%d\n", sizeof(boxes));

  printf("%d\n", 100000);

  printf("%d\n", sizeof(char));
  printf("%d\n", sizeof(short));
  printf("%d\n", sizeof(int));
  printf("%d\n", sizeof(long));
  printf("%d\n", sizeof(char*));
  printf("%d\n", sizeof(struct Book));
  printf("%d\n", sizeof(struct Book*));
  printf("%d\n", sizeof(union Box));
  printf("%d\n", sizeof(union Box*));
}
