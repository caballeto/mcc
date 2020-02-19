int func(int x) {
  switch (x * x) {
    case 1: return 0;
    case -2:
      switch (1) {
        case 1:
          return 0;
      }
      int x = 12;
    default: {
      return -1;
    }
    case 3: return 100;
    case 4: return 1000;
  }
}
