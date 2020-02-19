//
// Created by vladyslav on 18.02.20.
//

#include "Compiler.h"

namespace mcc {

bool Compiler::fDumpAst = false;
bool Compiler::fKeepAsm = false;
bool Compiler::fAssemble = false;
bool Compiler::fVerbose = false;
bool Compiler::fLink = true;

int Compiler::Compile(const std::string& input_file, const std::string& assembly) {
  std::ofstream out(assembly, std::ios::out);
  mcc::ErrorReporter reporter(std::cerr);
  reporter.SetFile(input_file);

  mcc::SymbolTable symbol_table;
  mcc::Scanner scanner(input_file, reporter);
  mcc::Parser parser(scanner, reporter);
  mcc::CodeGenX86 code_gen(out, symbol_table, reporter);
  mcc::TypeChecker type_checker(code_gen, reporter, symbol_table);

  mcc::AstDumper dumper(std::cout);

  std::vector<std::shared_ptr<mcc::Stmt>> stmts = parser.Parse();

  if (reporter.HadErrors()) {
    std::cerr << "Encountered " << reporter.errors_ << " syntax errors. Stopping compilation." << std::endl;
    exit(1);
  }

  if (Compiler::fVerbose) {
    std::cout << "-- parsed source code" << std::endl;
  }

  if (Compiler::fDumpAst) {
    std::cout << "-- dumping ast" << std::endl;
    dumper.Dump(stmts);
    dumper.Flush();
  }

  type_checker.TypeCheck(stmts);

  if (Compiler::fVerbose) {
    std::cout << "-- type checked statements" << std::endl;
  }

  if (reporter.HadErrors()) {
    std::cerr << "Encountered " << reporter.errors_ << " semantic errors. Stopping compilation." << std::endl;
    exit(1);
  }

  code_gen.Generate(stmts);

  if (Compiler::fVerbose) {
    std::cout << "-- generated assembly" << std::endl;
  }

  return 0;
}

} // namespace mcc