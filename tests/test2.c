void printint(long x);

int main() {
	int x;
	int y;
	
	y = 5;
	x = y = y + 1;

	printint(x);
	printint(y);
}