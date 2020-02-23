#include <stdio.h>

int main() {
	int i = 0;

	for (;;) {
	  if (i == 5) break;
	  ++i;
	}

	printf("%d\n", i);
}