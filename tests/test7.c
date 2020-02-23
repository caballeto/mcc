#include <stdio.h>

int main() {
	int x = 0;

	while (x < 10)
	  x = x + 1;

	printf("%d\n", x);

	do
	  x = x + 1;
	while (x < 25);

	printf("%d\n", x);

	for (; x < 40;)
	  x = x + 1;

	printf("%d\n", x);
}