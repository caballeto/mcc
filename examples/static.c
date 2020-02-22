static int x = 0;

static int func(int y) {
  return x * x + y * y;
}

int main() {
  x = 10;
  func(10);
  return 0;
}