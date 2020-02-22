void printint(long x);

typedef int* intptr;
typedef int Int;

Int main() {
  intptr x1, *x2, **x3;
  Int x = 10;
  x1 = &x;
  x2 = &x1;
  x3 = &x2;
  printint(***x3);
  return 0;
}