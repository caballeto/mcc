#include <stdio.h>

int main() {
    int i;
	for (i = 0; i < 5; i = i + 1) {
	  if (i == 2)
		continue;
	  printf("%d\n", i);
	}

	printf("%d\n", 100000);

	for (i = 0; i < 10; i = i + 1) {
	  if (i == 5)
		break;
	  printf("%d\n", i);
	}

	printf("%d\n", i);
}