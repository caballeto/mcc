//
// Created by vladyslav on 15.01.20.
//

#ifndef MCC_SRC_CODEGENX86_H_
#define MCC_SRC_CODEGENX86_H_

#include "common.h"
#include "ast.h"
#include "Visitor.h"
#include "SymbolTable.h"
#include "ControlFlowChecker.h"
#include "ErrorReporter.h"

#define REGISTER_NUM 8
#define NO_RETURN_REGISTER -1

namespace mcc {

class CodeGenX86: public Visitor {
 public:
  explicit CodeGenX86(const std::string& output_file, SymbolTable& symbol_table, ErrorReporter& reporter)
    : symbol_table_(symbol_table), reporter_(reporter) {
    out_.open(output_file, std::ios::out);
    label_ = 0;
  }

  void Generate(const std::vector<std::shared_ptr<Stmt>>& stmts);

  int Visit(const std::shared_ptr<Binary>& binary) override;
  int Visit(const std::shared_ptr<Literal>& literal) override;
  int Visit(const std::shared_ptr<Assign>& assign) override;
  int Visit(const std::shared_ptr<VarDecl>& var_decl) override;
  int Visit(const std::shared_ptr<Print>& print) override;
  int Visit(const std::shared_ptr<ExpressionStmt>& expr_stmt) override;
  int Visit(const std::shared_ptr<Conditional>& cond_stmt) override;
  int Visit(const std::shared_ptr<Block>& block_stmt) override;
  int Visit(const std::shared_ptr<While>& while_stmt) override;
  int Visit(const std::shared_ptr<For>& for_stmt) override;
  int Visit(const std::shared_ptr<DeclList>& decl_list) override;
  int Visit(const std::shared_ptr<ExprList>& expr_list) override;
  int Visit(const std::shared_ptr<ControlFlow>& flow_stmt) override;

 private:
  int GetLabel();

  void Preamble();
  void Postamble();

  int NewRegister();
  void FreeRegister(int reg);
  void PrintInt(int reg);

  static std::string GetSetInstr(TokenType type);

  ErrorReporter& reporter_;
  std::stack<std::pair<std::string, std::string>> loop_stack_;
  int label_;
  std::ofstream out_;
  ControlFlowChecker flow_checker_;
  SymbolTable& symbol_table_;

  const std::vector<std::string> kRegisters = {"%r8", "%r9", "%r10", "%r11",
                                               "%r12", "%r13", "%r14", "%r15"};
  const std::vector<std::string> kDregisters = {"%r8d", "%r9d", "%r10d", "%r11d",
                                                "%r12d", "%r13d", "%r14d", "%r15d"};

  const std::vector<std::string> kBregisters = {"%r8b", "%r9b", "%r10b", "%r11b",
                                                "%r12b", "%r13b", "%r14b", "%r15b"};

  bool regs_status[REGISTER_NUM] = {true, true, true, true, true, true, true, true};
};

} // namespace mcc

#endif //MCC_SRC_CODEGENX86_H_
