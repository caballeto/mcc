typedef struct {
  int length;
  char* title;
  char* author;
} Book;

int main() {
  Book book;

  long x = 25;

  int ptr = (Book*) x;
}
