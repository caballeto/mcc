void printint(long x);

int main() {
	int **p1, *p2, x;

	x = 5;
	p2 = &x;

	printint(x);
	printint(*p2);

	*p2 = 15;

	printint(x);
	printint(*p2);

	p1 = &p2;

	**p1 = 25;

	printint(x);
	printint(**p1);
}