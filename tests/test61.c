#include <stdio.h>

typedef int Int;
typedef Int* IntPtr;
typedef Int** IntPtr2;

int main() {
  Int x = 1;
  IntPtr y = &x;
  IntPtr2 z = &y;

  printf("%d\n", **z);

  return 0;
}