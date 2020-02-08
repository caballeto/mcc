int* func() {
  return 0;
}

int main() {
  char* c;
  c = 25;
  int*** d = 34;
  int** m;
  m = d;
  int** l;
  l = c;
  int z = d;
  char f = m;
  int** x = &z;
  *(*x)++ = 10;
  (*((*x)++))++ = 10;
  *x++ = 25;
  (*x)++++; // addl $1 (%r1)
  (*(&z + 1))++;
  (*x + 1)++;
  (*func())++;
  int k = &(&(*x));
  return 0;
}