
int main() {
  int x = 10;
  if (x == 10) {
    x++;
    goto failed;
  }

  return 0;
}