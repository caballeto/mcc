#include <iostream>

#include "src/common.h"
#include "src/Token.h"
#include "src/TokenType.h"
#include "src/Scanner.h"
#include "src/Parser.h"
#include "src/ast.h"
#include "src/CodeGenX86.h"
#include "src/Visitor.h"
#include "src/SymbolTable.h"
#include "src/AstDumper.h"

#define AST_DUMP_FILE "ast_dump.txt"
#define ASSEMBY_FILE "out.s"

void Usage() {
  std::cout << "Usage: " << std::endl;
  std::cout << "mcc file.c" << std::endl;
}

//#TODO: add DEBUG macro for debug messages
int main(int argc, char* argv[]) {
  if (argc != 2) {
    Usage();
    exit(1);
  }

  std::string input_file = std::string(argv[1]);

  mcc::SymbolTable symbol_table;
  mcc::Scanner scanner(input_file);
  mcc::Parser parser(scanner, symbol_table);
  mcc::CodeGenX86 code_gen(ASSEMBY_FILE, symbol_table);
  mcc::AstDumper dumper(AST_DUMP_FILE);

  std::vector<std::shared_ptr<mcc::Stmt>> stmts = parser.Parse();

  std::cout << "-- generated tree" << std::endl;

  dumper.Dump(stmts);
  dumper.Flush();

  std::cout << "-- dumped tree into '" << AST_DUMP_FILE << "'" << std::endl;

  code_gen.Generate(stmts);

  std::cout << "-- generated assembly into '" << ASSEMBY_FILE << "'" << std::endl;

  return 0;
}