void printint(long x);

static int square(int x) {
  return x * x;
}

static int z;

int k;

int main() {
  k = 0;
  z = square(10);
  printint(z);
  printint(k);
  return 0;
}