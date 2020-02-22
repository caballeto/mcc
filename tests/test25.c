void printint(long x);

int main() {
  int *x, y;
  x = &y;

  long z = x;

  printint(x == z);
  printint(x + 2 == z + 8);
  printint(x - 2 == z - 8);

  x++;
  z = z + 4;

  printint(x == z);

  x--;
  z = z - 4;

  printint(x == z);

  return 0;
}