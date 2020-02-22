int main() {
  typedef char Char;
  typedef int Int;
  typedef long Long;

  Char c = 'a';
  Int z = 10;
  Long x = 10000;

  c = z;
  z = x;

  return 0;
}