void printint(long x);

int array[20];

int main() {
  int* ptr = array;
  *ptr = 10;
  *(ptr + 1) = 20;
  printint(array[0]);
  printint(array[1]);
}