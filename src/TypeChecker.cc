//
// Created by vladyslav on 03.02.20.
//

#include "ErrorReporter.h"
#include "TypeChecker.h"
#include "Visitor.h"

namespace mcc {

// Rewrite ErrorReporter to support different error reporting
// Implement type checking for new types (short, int, long, void)
// Implement code generation for new types

// for tomorrow : big refactoring
// parser synchronization on errors
// error reporter to print tokens, line number, character counts
// support for pointers of different types
// testing routines + tests with gtest

bool TypeChecker::IsPointer(ExprRef expr) {
  return expr->indirection > 0;
}

bool TypeChecker::IsIntegerType(ExprRef expr) {
  return expr->type_ == Type::INT || expr->type_ == Type::SHORT || expr->type_ == Type::LONG;
}

// #FIXME: add support for pointers/structs/unions
bool TypeChecker::MatchType(Type type, int indirection, ExprRef expr) {
  if (expr->type_ == Type::VOID) {
    reporter_.Report("'void' type could not be assigned to any variable ");
    return false;
  }

  if (type == expr->type_) return true;
  int t1 = (int) type, t2 = (int) expr->type_;

  if (t1 < t2) {
    reporter_.ReportSemanticError("Could not cast right expression", type, expr->type_);
    return false;
  }

  return true;
}

// #TODO: build type hierarchy as a separate structure and provide API for type casting
// assuming types are not void
Type TypeChecker::MatchTypes(ExprRef e1, ExprRef e2, bool to_left) {
  if (e1->type_ == Type::VOID || e2->type_ == Type::VOID) {
    reporter_.ReportSemanticError("Invalid operand types in binary expressions", e1->type_, e2->type_);
    return Type::NONE;
  }

  if (e1->type_ == e2->type_) return e1->type_;

  if (!IsIntegerType(e1) || !IsIntegerType(e2)) {
    reporter_.ReportSemanticError("Invalid operand types in binary expressions", e1->type_, e2->type_);
    return Type::NONE;
  }

  return to_left ? PromoteToLeft(e1, e2) : Promote(e1, e2);
}

Type TypeChecker::PromoteToLeft(ExprRef e1, ExprRef e2) {
  int t1 = (int) e1->type_, t2 = (int) e2->type_;

  if (t1 < t2) {
    reporter_.ReportSemanticError("Could not cast right expression", e1->type_, e2->type_);
    return Type::NONE;
  }

  return e1->type_;
}

Type TypeChecker::Promote(ExprRef e1, ExprRef e2) {
  int t1 = (int) e1->type_, t2 = (int) e2->type_;
  return (Type) std::max(t1, t2);
}

Type TypeChecker::Visit(const std::shared_ptr<Binary>& binary) {
  binary->left_->Accept(*this);
  binary->right_->Accept(*this);
  binary->type_ = MatchTypes(binary->left_, binary->right_, false);
  return binary->type_;
}

Type TypeChecker::Visit(const std::shared_ptr<Assign>& assign) {
  if (!assign->left_->IsLvalue()) {
    reporter_.Report("Assign to rvalue is not allowed.");
    return Type::NONE;
  }

  assign->left_->Accept(*this);
  assign->right_->Accept(*this);
  assign->type_ = MatchTypes(assign->left_, assign->right_, true);
  return assign->type_;
}

void TypeChecker::TypeCheck(const std::vector<std::shared_ptr<Stmt>>& stmts) {
  for (const auto& stmt : stmts) {
    stmt->Accept(*this);
  }
}

Type TypeChecker::Visit(const std::shared_ptr<Literal>& literal) {
  if (literal->IsVariable()) {
    int id = symbol_table_.Get(literal->literal_->GetStringValue());

    if (id == -1) {
      reporter_.Report("Variable has not been declared '"
        + literal->literal_->GetStringValue() + "'.", literal->literal_);
      return Type::NONE;
    }

    literal->type_ = symbol_table_.Get(id).type_;
    return literal->type_;
  } else {
    int val = literal->literal_->GetIntValue();
    literal->type_ = (val <= 255 && val >= -256) ? Type::SHORT : Type::INT;
    return literal->type_;
  }
}

// #TODO: fix error reporting
Type TypeChecker::Visit(const std::shared_ptr<Print>& print) {
  print->expr_->Accept(*this);

  if (!IsIntegerType(print->expr_)) {
    reporter_.Report("'print' statement expect integer.");
  }

  return Type::NONE;
}

Type TypeChecker::Visit(const std::shared_ptr<ExpressionStmt>& expr_stmt) {
  expr_stmt->expr_->Accept(*this);
  return Type::NONE;
}

Type TypeChecker::Visit(const std::shared_ptr<Conditional>& cond_stmt) {
  cond_stmt->condition_->Accept(*this);

  if (!IsIntegerType(cond_stmt->condition_) && !IsPointer(cond_stmt->condition_)) {
    reporter_.Report("Used not-scalar where scalar is required");
    return Type::NONE;
  }

  cond_stmt->then_block_->Accept(*this);

  if (cond_stmt->else_block_ != nullptr) {
    cond_stmt->else_block_->Accept(*this);
  }

  return Type::NONE;
}

Type TypeChecker::Visit(const std::shared_ptr<Block>& block_stmt) {
  for (const auto& stmt : block_stmt->stmts_)
    stmt->Accept(*this);
  return Type::NONE;
}

Type TypeChecker::Visit(const std::shared_ptr<While>& while_stmt) {
  while_stmt->condition_->Accept(*this);

  if (!IsIntegerType(while_stmt->condition_) && !IsPointer(while_stmt->condition_)) {
    reporter_.Report("Used not-scalar where scalar is required");
    return Type::NONE;
  }

  while_stmt->loop_block_->Accept(*this);
  return Type::NONE;
}

Type TypeChecker::Visit(const std::shared_ptr<For>& for_stmt) {
  for_stmt->init_->Accept(*this);
  for_stmt->condition_->Accept(*this);

  if (!IsIntegerType(for_stmt->condition_) && !IsPointer(for_stmt->condition_)) {
    reporter_.Report("Used not-scalar where scalar is required");
    return Type::NONE;
  }

  for_stmt->loop_block_->Accept(*this);
  for_stmt->update_->Accept(*this);
  return Type::NONE;
}

Type TypeChecker::Visit(const std::shared_ptr<DeclList>& decl_list) {
  for (const auto& var_decl : decl_list->var_decl_list_)
    var_decl->Accept(*this);
  return Type::NONE;
}

Type TypeChecker::Visit(const std::shared_ptr<ExprList>& expr_list) {
  for (const auto& expr : expr_list->expr_list_)
    expr->Accept(*this);
  return Type::NONE;
}

Type TypeChecker::Visit(const std::shared_ptr<ControlFlow>& flow_stmt) {
  return Type::NONE;
}

Type TypeChecker::Visit(const std::shared_ptr<VarDecl>& decl) {
  int id = symbol_table_.Get(decl->name_->GetStringValue());

  if (decl->var_type_ == Type::VOID) {
    reporter_.Report("Variable '" + decl->name_->GetStringValue() + "' has invalid type 'void'");
    return Type::NONE;
  }

  if (id == -1) {
    id = symbol_table_.Put(decl->name_->GetStringValue(), decl->var_type_, decl->indirection_);
  } else {
    reporter_.Report("Variable '" + decl->name_->GetStringValue() + "' has already been declared");
    return Type::NONE;
  }

  if (decl->init_ != nullptr) {
    decl->init_->Accept(*this);
    if (!MatchType(decl->var_type_, decl->indirection_, decl->init_)) {
      reporter_.ReportSemanticError("The type of init expression does not match the declared type of the variable ",
          decl->var_type_, decl->init_->type_);
    }
  }

  return Type::NONE;
}

} // namespace mcc
