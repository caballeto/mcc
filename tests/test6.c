#include <stdio.h>

int main() {
	for (int i = 0, j = 0; i < 10; i++, j++) {
	  printf("%d\n", i);
	  printf("%d\n", j);
	  printf("%d\n", i + j);
	}
}