int While(int x) {
  do {
    x++;
  } while (x < 10);
  return x;
}

int For(int x) {
  int y = 0, j = 0;
  for (int y = 1, j = 1; x < 10; y++) {
    x += y;
  }
  return x;
}

int main() {
  While(0);
  return 0;
}