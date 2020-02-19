void printint(long x);

int main() {
  int array[20];
  array[0] = 5;
  printint(array[0]);
  array[0]++;
  printint(array[0]);
  array[0]--;
  printint(array[0]);

  int z = array[0];
  printint(z);
}