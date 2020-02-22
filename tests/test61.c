void printint(long x);

typedef int Int;
typedef Int* IntPtr;
typedef Int** IntPtr2;

int main() {
  Int x = 1;
  IntPtr y = &x;
  IntPtr2 z = &y;

  printint(**z);

  return 0;
}