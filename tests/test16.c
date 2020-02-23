#include <stdio.h>

int main() {
	int x = 25, y = x * 4;

	long bigger = 1000;

	x = bigger;

	printf("%d\n", x);
	printf("%ld\n", bigger);
}