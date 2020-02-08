int func(int x, int y) {
  int z = x && y + 1;
  return z;
}

int main() {
  func(0, 0);
  func(1, 1);
  func(1, 0);
  return 0;
}