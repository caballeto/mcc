#include <stdio.h>

int main() {
	int x;
	int y;
	
	y = 5;
	x = y = y + 1;

	printf("%d\n", x);
	printf("%d\n", y);
}