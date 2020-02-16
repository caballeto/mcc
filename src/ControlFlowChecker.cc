//
// Created by vladyslav on 03.02.20.
//

#include "ControlFlowChecker.h"

namespace mcc {

int ControlFlowChecker::Visit(const std::shared_ptr<Binary>& binary) {
  return 0;
}

int ControlFlowChecker::Visit(const std::shared_ptr<Literal>& literal) {
  return 0;
}

int ControlFlowChecker::Visit(const std::shared_ptr<Assign>& assign) {
  return 0;
}

int ControlFlowChecker::Visit(const std::shared_ptr<VarDecl>& var_decl) {
  return 0;
}

int ControlFlowChecker::Visit(const std::shared_ptr<Print>& print) {
  return 0;
}

int ControlFlowChecker::Visit(const std::shared_ptr<ExpressionStmt>& expr_stmt) {
  return 0;
}

int ControlFlowChecker::Visit(const std::shared_ptr<Conditional>& cond_stmt) {
  return cond_stmt->then_block_->Accept(*this) || cond_stmt->else_block_->Accept(*this);
}

int ControlFlowChecker::Visit(const std::shared_ptr<Block>& block_stmt) {
  for (const auto& stmt : block_stmt->stmts_)
    if (stmt->Accept(*this))
      return 1;
  return 0;
}

int ControlFlowChecker::Visit(const std::shared_ptr<While>& while_stmt) {
  return 0;
}

int ControlFlowChecker::Visit(const std::shared_ptr<For>& for_stmt) {
  return 0;
}

int ControlFlowChecker::Visit(const std::shared_ptr<DeclList>& decl_list) {
  return 0;
}

int ControlFlowChecker::Visit(const std::shared_ptr<ExprList>& expr_list) {
  return 0;
}

int ControlFlowChecker::Visit(const std::shared_ptr<ControlFlow>& flow_stmt) {
  return 1;
}

int ControlFlowChecker::Visit(const std::shared_ptr<Unary>& unary) {
  return 0;
}

int ControlFlowChecker::Visit(const std::shared_ptr<FuncDecl>& func_decl) {
  return 0;
}

int ControlFlowChecker::Visit(const std::shared_ptr<Return>& return_stmt) {
  return 0;
}

int ControlFlowChecker::Visit(const std::shared_ptr<Call>& call) {
  return 0;
}

int ControlFlowChecker::Visit(const std::shared_ptr<Grouping> &grouping) {
  return 0;
}

int ControlFlowChecker::Visit(const std::shared_ptr<Ternary> &ternary) {
  return 0;
}

int ControlFlowChecker::Visit(const std::shared_ptr<Postfix> &postfix) {
  return 0;
}

int ControlFlowChecker::Visit(const std::shared_ptr<Index> &index) {
  return 0;
}

int ControlFlowChecker::Visit(const std::shared_ptr<Label> &label) {
  return 0;
}

int ControlFlowChecker::Visit(const std::shared_ptr<GoTo> &go_to) {
  return 0;
}

int ControlFlowChecker::Visit(const std::shared_ptr<Struct> &struct_decl) {
  return 0;
}

int ControlFlowChecker::Visit(const std::shared_ptr<Access> &access) {
  return 0;
}

} // namespace mcc
