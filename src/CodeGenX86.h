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

class CodeGenX86: public Visitor<int> {
 public:
  CodeGenX86(const std::string& output_file, SymbolTable& symbol_table, ErrorReporter& reporter)
    : symbol_table_(symbol_table), reporter_(reporter) {
    out_.open(output_file, std::ios::out);
    label_ = 0;
    return_label_ = -1;

    type_sizes_[Type::INT] = 4;
    type_sizes_[Type::SHORT] = 2;
    type_sizes_[Type::LONG] = 8;
  }

  void Generate(const std::vector<std::shared_ptr<Stmt>>& stmts);

  int Visit(const std::shared_ptr<Binary>& binary) override;
  int Visit(const std::shared_ptr<Unary>& unary) override;
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
  int Visit(const std::shared_ptr<FuncDecl>& func_decl) override;
  int Visit(const std::shared_ptr<Return>& return_stmt) override;
  int Visit(const std::shared_ptr<Call>& call) override;
  int Visit(const std::shared_ptr<Index> &index) override;
  int Visit(const std::shared_ptr<Grouping> &grouping) override;
  int Visit(const std::shared_ptr<Ternary> &ternary) override;
  int Visit(const std::shared_ptr<Postfix> &postfix) override;

 private:
  int GetLabel();

  void Preamble();
  void Postamble();

  int NewRegister();
  void FreeRegister(int reg);
  void PrintInt(int reg);

  static std::string GetSetInstr(TokenType type);

  int GetTypeSize(Type type, int ind);
  static std::string GetAllocType(Type type, int ind);
  std::string GetPostfix(Type type, int ind);
  std::string GetRegister(int r, Type type, int ind);

  std::map<Type, int> type_sizes_;
  ErrorReporter& reporter_;
  std::stack<std::pair<std::string, std::string>> loop_stack_; // FIXME: move to type checker

  int label_;
  int return_label_;

  std::ofstream out_;
  ControlFlowChecker flow_checker_; // FIXME: move to type checker
  SymbolTable& symbol_table_;

  const std::vector<std::string> kRegisters = {"%r8", "%r9", "%r10", "%r11",
                                               "%r12", "%r13", "%r14", "%r15"};
  const std::vector<std::string> kDregisters = {"%r8d", "%r9d", "%r10d", "%r11d",
                                                "%r12d", "%r13d", "%r14d", "%r15d"};
  const std::vector<std::string> kWregisters = {"%r8w", "%r9w", "%r10w", "%r11w",
                                                "%r12w", "%r13w", "%r14w", "%r15w"};
  const std::vector<std::string> kBregisters = {"%r8b", "%r9b", "%r10b", "%r11b",
                                                "%r12b", "%r13b", "%r14b", "%r15b"};

  bool regs_status[REGISTER_NUM] = {true, true, true, true, true, true, true, true};
};

} // namespace mcc

#endif //MCC_SRC_CODEGENX86_H_
