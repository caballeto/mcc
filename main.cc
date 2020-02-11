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
#include "src/ErrorReporter.h"
#include "src/TypeChecker.h"

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
  std::ofstream out(ASSEMBY_FILE, std::ios::out);
  mcc::ErrorReporter reporter(std::cerr);
  reporter.SetFile(input_file);

  mcc::SymbolTable symbol_table;
  mcc::Scanner scanner(input_file, reporter);
  mcc::Parser parser(scanner, reporter);
  mcc::CodeGenX86 code_gen(out, symbol_table, reporter);
  mcc::TypeChecker type_checker(code_gen, reporter, symbol_table);
  mcc::AstDumper dumper(AST_DUMP_FILE);

  symbol_table.PutGlobal("printint", mcc::Type::VOID, 0, 0, true);
  symbol_table.PutGlobal("printchar", mcc::Type::VOID, 0, 0, true);

  std::vector<std::shared_ptr<mcc::Stmt>> stmts = parser.Parse();

  if (reporter.HadErrors()) {
    std::cerr << "Encountered " << reporter.errors_ << " syntax errors. Stopping compilation." << std::endl;
    exit(1);
  }

  // std::cout << "-- generated tree" << std::endl;

  dumper.Dump(stmts);
  dumper.Flush();

  // std::cout << "-- dumped tree into '" << AST_DUMP_FILE << "'" << std::endl;

  type_checker.TypeCheck(stmts);

  if (reporter.HadErrors()) {
    std::cerr << "Encountered " << reporter.errors_ << " semantic errors. Stopping compilation." << std::endl;
    exit(1);
  }

  // std::cout << "-- type checked statements" << std::endl;

  code_gen.Generate(stmts);

  // std::cout << "-- generated assembly into '" << ASSEMBY_FILE << "'" << std::endl;

  return 0;
}