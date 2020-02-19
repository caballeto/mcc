void printint(long x);

int func() {
  int y = 5;
  if (y == 5) {
    return 25;
  } else {
    return 1;
  }
}

int main() {
  int z = func();
  printint(z);
  return 0;
}