void printint(long x);

int func(int x) {
  if (x == 25) {
    goto result;
  }

  printint(x - 25);
  return x - 25;

result:
  printint(x);
  return x;
}

int main() {
	func(0);
	func(25);
}