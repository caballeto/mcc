void printint(long x);

int main() {
  int *x;
  int z = 0;

  x = &z;
  *x = 5;

  printint((*x)--);
  printint(*x);
  printint(--(*x));
  printint(*x);

  return 0;
}