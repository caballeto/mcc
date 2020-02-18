void printint(long x);

int array[20];

int main() {
  int* ptr = &array[0];
  *ptr = 10;
  *(ptr + 1) = 20;
  ptr[2] = 30;
  *(array + 3) = 40;

  printint(array[0]);
  printint(array[1]);
  printint(array[2]);
  printint(array[3]);

  printint(ptr[0]);
  printint(*ptr);
  printint(ptr[1]);
  printint(*(ptr + 1));
  printint(ptr[2]);
  printint(*(ptr + 2));
  printint(ptr[3]);
  printint(*(ptr + 3));
}
