void printint(long x);

int main() {
	int x = 0;

	while (x < 10)
	  x = x + 1;

	printint(x);

	do
	  x = x + 1;
	while (x < 25);

	printint(x);

	for (; x < 40;)
	  x = x + 1;

	printint(x);
}