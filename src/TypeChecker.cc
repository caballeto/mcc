//
// Created by vladyslav on 03.02.20.
//

#include "TypeChecker.h"

namespace mcc {

int TypeChecker::Visit(const std::shared_ptr<Binary> &binary) {
  return 0;
}

int TypeChecker::Visit(const std::shared_ptr<Literal> &literal) {
  return 0;
}

int TypeChecker::Visit(const std::shared_ptr<Assign> &assign) {
  return 0;
}

int TypeChecker::Visit(const std::shared_ptr<VarDecl> &var_decl) {
  return 0;
}

int TypeChecker::Visit(const std::shared_ptr<Print> &print) {
  return 0;
}

int TypeChecker::Visit(const std::shared_ptr<ExpressionStmt> &expr_stmt) {
  return 0;
}

int TypeChecker::Visit(const std::shared_ptr<Conditional> &cond_stmt) {
  return 0;
}

int TypeChecker::Visit(const std::shared_ptr<Block> &block_stmt) {
  return 0;
}

int TypeChecker::Visit(const std::shared_ptr<While> &while_stmt) {
  return 0;
}

int TypeChecker::Visit(const std::shared_ptr<For> &for_stmt) {
  return 0;
}

int TypeChecker::Visit(const std::shared_ptr<DeclList> &decl_list) {
  return 0;
}

int TypeChecker::Visit(const std::shared_ptr<ExprList> &expr_list) {
  return 0;
}

int TypeChecker::Visit(const std::shared_ptr<ControlFlow> &flow_stmt) {
  return 0;
}

void TypeChecker::TypeCheck(const std::vector<std::shared_ptr<Stmt>>& stmts) {
  for (const auto& stmt : stmts) {
    stmt->Accept(*this);
  }
}

} // namespace mcc
