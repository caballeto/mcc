//
// Created by vladyslav on 03.02.20.
//

#include "ControlFlowChecker.h"

namespace mcc {

int ControlFlowChecker::Visit(Binary& binary) {
  return 0;
}

int ControlFlowChecker::Visit(Literal& literal) {
  return 0;
}

int ControlFlowChecker::Visit(Assign& assign) {
  return 0;
}

int ControlFlowChecker::Visit(VarDecl& var_decl) {
  return 0;
}

int ControlFlowChecker::Visit(Print& print) {
  return 0;
}

int ControlFlowChecker::Visit(ExpressionStmt& expr_stmt) {
  return 0;
}

int ControlFlowChecker::Visit(Conditional& cond_stmt) {
  return cond_stmt.then_block_->Accept(*this) || cond_stmt.else_block_->Accept(*this);
}

int ControlFlowChecker::Visit(Block& block_stmt) {
  for (const auto& stmt : block_stmt.stmts_)
    if (stmt->Accept(*this))
      return 1;
  return 0;
}

int ControlFlowChecker::Visit(While& while_stmt) {
  return 0;
}

int ControlFlowChecker::Visit(For& for_stmt) {
  return 0;
}

int ControlFlowChecker::Visit(DeclList& decl_list) {
  return 0;
}

int ControlFlowChecker::Visit(ExprList& expr_list) {
  return 0;
}

int ControlFlowChecker::Visit(ControlFlow& flow_stmt) {
  return 1;
}

int ControlFlowChecker::Visit(Unary& unary) {
  return 0;
}

int ControlFlowChecker::Visit(FuncDecl& func_decl) {
  return 0;
}

int ControlFlowChecker::Visit(Return& return_stmt) {
  return 0;
}

int ControlFlowChecker::Visit(Call& call) {
  return 0;
}

int ControlFlowChecker::Visit(Grouping &grouping) {
  return 0;
}

int ControlFlowChecker::Visit(Ternary &ternary) {
  return 0;
}

int ControlFlowChecker::Visit(Postfix &postfix) {
  return 0;
}

int ControlFlowChecker::Visit(Index &index) {
  return 0;
}

int ControlFlowChecker::Visit(Label &label) {
  return 0;
}

int ControlFlowChecker::Visit(GoTo &go_to) {
  return 0;
}

int ControlFlowChecker::Visit(Struct &struct_decl) {
  return 0;
}

int ControlFlowChecker::Visit(Access &access) {
  return 0;
}

int ControlFlowChecker::Visit(Union &union_decl) {
  return 0;
}

int ControlFlowChecker::Visit(Enum &enum_decl) {
  return 0;
}

int ControlFlowChecker::Visit(Switch &switch_stmt) {
  return 0;
}

int ControlFlowChecker::Visit(Typedef &typedef_stmt) {
  return 0;
}

int ControlFlowChecker::Visit(TypeCast &type_cast) {
  return 0;
}

int ControlFlowChecker::Visit(Logical &logical) {
  return 0;
}

} // namespace mcc
