void printint(long x);

int main() {
	int x;
	int y;

	x = 5;
	y = 6;

	printint(x + y);
	printint(x);
	printint(y);

	x = y;
	printint(x);
	printint(y);

	printint(1 + 2 * 5 + k);
}