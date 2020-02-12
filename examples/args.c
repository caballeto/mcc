short myfunc(short a, short b, short c, short d,
            short e, short f, short g, short h)
{
  short xx = a * b * c * d * e * f * g * h;
  short yy = a + b + c + d + e + f + g + h;
  return yy + 20;
}

int main() {
  short x = myfunc(1, 2, 3, 4, 5, 6, 7, 8);
  return x;
}