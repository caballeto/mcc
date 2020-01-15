#include <iostream>

#include "src/common.h"
#include "src/Token.h"
#include "src/TokenType.h"
#include "src/Scanner.h"
#include "src/Parser.h"
#include "src/ast.h"
#include "src/CodeGenX86.h"

#define AST_DUMP_FILE "ast_dump.txt"
#define ASSEMBY_FILE "out.s"

void Usage() {
  std::cout << "Usage: " << std::endl;
  std::cout << "mcc file.c" << std::endl;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    Usage();
    exit(1);
  }

  std::string input_file = std::string(argv[1]);

  mcc::Scanner scanner(input_file);
  mcc::Parser parser(scanner);
  mcc::CodeGenX86 code_gen(ASSEMBY_FILE);

  std::ofstream ast_dump(AST_DUMP_FILE, std::ios::out);

  std::shared_ptr<mcc::Expr> expr = parser.Expression(0);

  expr->Dump(ast_dump, 0);

  code_gen.Generate(expr);

  return 0;
}