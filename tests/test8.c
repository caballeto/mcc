void printint(long x);

int main() {
    int i;
	for (i = 0; i < 5; i = i + 1) {
	  if (i == 2)
		continue;
	  printint(i);
	}

	printint(100000);

	for (i = 0; i < 10; i = i + 1) {
	  if (i == 5)
		break;
	  printint(i);
	}

	printint(i);
}