#include <stdio.h>

int main() {
	int i;

	i = 0;

	while (i < 10) {
	  printf("%d\n", i);
	  i = i + 1;
	}

	do {
	  printf("%d\n", i);
	  i = i - 1;
	} while (i >= 0);

	for (i = 0; i < 25; i = i + 2) {
	  printf("%d\n", i);
	}
}
