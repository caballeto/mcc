void printint(long x);

int main() {
	int x = 25, y = x * 4;

	long bigger = 1000;

	x = bigger;

	printint(x);
	printint(bigger);
}