void printchar(long x);

int main() {
  int x = 25;
  char* c = (char*) &x;
  *c = 'a';
  printchar(*c);
  return 0;
}