typedef struct {
  int length;
  char* title;
  char* author;
} Book;

int main() {
  Book book;

  void* ptr = (void*) &book;

  *ptr;

  int* x = (int*) book;
  long* y = (Book*) book;

  return 0;
}