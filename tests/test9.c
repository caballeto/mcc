void printint(long x);

int main() {
	int i = 0;

	for (;;) {
	  if (i == 5) break;
	  ++i;
	}

	printint(i);
}