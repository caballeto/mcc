void printint(long x);

int array[20];

int main() {
  array[0] = 5;
  printint(array[0]);
  array[0]++;
  printint(array[0]);
  array[0]--;
  printint(array[0]);
}