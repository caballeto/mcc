#include <stdio.h>

int main() {
	int **p1, *p2, x;

	x = 5;
	p2 = &x;

	printf("%d\n", x);
	printf("%d\n", *p2);

	*p2 = 15;

	printf("%d\n", x);
	printf("%d\n", *p2);

	p1 = &p2;

	**p1 = 25;

	printf("%d\n", x);
	printf("%d\n", **p1);
}
