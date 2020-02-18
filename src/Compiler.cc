//
// Created by vladyslav on 18.02.20.
//

#include "Compiler.h"

namespace mcc {

int Compiler::Compile(const std::string& input_file, const std::string& assembly, const std::string& dump) {
  std::ofstream out(assembly, std::ios::out);
  mcc::ErrorReporter reporter(std::cerr);
  reporter.SetFile(input_file);

  mcc::SymbolTable symbol_table;
  mcc::Scanner scanner(input_file, reporter);
  mcc::Parser parser(scanner, reporter);
  mcc::CodeGenX86 code_gen(out, symbol_table, reporter);
  mcc::TypeChecker type_checker(code_gen, reporter, symbol_table);
  mcc::AstDumper dumper(dump);

  std::vector<std::shared_ptr<mcc::Stmt>> stmts = parser.Parse();

  if (reporter.HadErrors()) {
    std::cerr << "Encountered " << reporter.errors_ << " syntax errors. Stopping compilation." << std::endl;
    exit(1);
  }

  dumper.Dump(stmts);
  dumper.Flush();

  type_checker.TypeCheck(stmts);

  if (reporter.HadErrors()) {
    std::cerr << "Encountered " << reporter.errors_ << " semantic errors. Stopping compilation." << std::endl;
    exit(1);
  }

  code_gen.Generate(stmts);

  return 0;
}

} // namespace mcc