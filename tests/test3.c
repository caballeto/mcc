void printint(long x);

int main() {
	int x;
	int y;

	x = 5;
	y = 6;

	printint(x >= y);
	printint(x == y);
	printint(x != y);
	printint(x < y);
	printint(x > y);
	printint(x <= y);
}