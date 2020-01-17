int compare(int x, int y) {
  int a = x == y;
  int b = x != y;
  int c = x < y;
  int d = x <= y;
  int e = x > y;
  int f = x >= y;
  return (((((a + b) + c) + d) + e) + f);
}

int main() {
  return 0;
}