void printint(long x);

int main() {
  int x = 0;
  ++x;
  printint(x);
  printint(++x);
  printint(x);
  return 0;
}