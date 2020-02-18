#include <iostream>

#include "src/Compiler.h"

#define AST_DUMP_FILE "ast_dump.txt"
#define ASSEMBY_FILE "out.s"

void Usage() {
  std::cout << "Usage: " << std::endl;
  std::cout << "mcc file.c" << std::endl;
}

//#TODO: add DEBUG macro for debug messages
//#TODO: add Compiler class to initialize all the stuff
int main(int argc, char* argv[]) {
  //if (argc > 2) {
  //  Usage();
  //  exit(1);
  //}

  std::string input_file = std::string(argv[1]);
  mcc::Compiler::Compile(input_file, ASSEMBY_FILE, AST_DUMP_FILE);

  return 0;
}