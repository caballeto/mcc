void printint(long x);

int a;
int b[25];

int main() {
  b[3] = 12;
  a = b[3];
  printint(a);
  printint(b[3]);
  return 0;
}