int z = 1;

int func(int x) {
  x = (x--) + x + 2 * (x--) + z;
  return x;
}

int main() {
  int *p;
  return 0;
}