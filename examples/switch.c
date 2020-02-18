int func(int x) {
  switch (x * x) {
    case 1: return 0;
    case 2: return 10;
    case 3: return 100;
    case 4: return 1000;
    default: {
      return -1;
    }
  }
}
