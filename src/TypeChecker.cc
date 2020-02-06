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
  if (expr == nullptr) return false;
  return expr->indirection_ > 0;
}

bool TypeChecker::IsIntegerType(ExprRef expr) {
  if (expr == nullptr) return false;
  return expr->type_ == Type::INT || expr->type_ == Type::SHORT || expr->type_ == Type::LONG;
}

bool TypeChecker::MatchTypeInit(Type type, int indirection, ExprRef init) {
  if (indirection == 0) {
    return IsPointer(init) ? true : (int) type >= (int) init->type_;
  } else {
    return true;
  }
}

// #TODO: build type hierarchy as a separate structure and provide API for type casting
// assuming types are not void
Type TypeChecker::MatchTypes(ExprRef e1, ExprRef e2, bool to_left, ExprRef binary) {
  if (e1->type_ == Type::VOID || e2->type_ == Type::VOID) {
    reporter_.ReportSemanticError("Invalid operands types in binary expression, 'void' is not allowed",
        e1, e2, binary->op_);
    return Type::NONE;
  }

  if (IsPointer(e1) && IsPointer(e2)) {
    return MatchPointers(e1, e2, to_left, binary);
  } else if (!IsPointer(e1) && !IsPointer(e2)) {
    return MatchPrimitives(e1, e2, to_left, binary);
  } else {
    return MatchMixed(e1, e2, to_left, binary);
  }
}

Type TypeChecker::MatchPointers(ExprRef e1, ExprRef e2, bool to_left, ExprRef binary) {
  if (to_left && binary->op_->GetType() == TokenType::T_ASSIGN) {
    // FIXME: issue warnings on incompatible pointer casts
    binary->type_ = e1->type_;
    binary->indirection_ = e1->indirection_;
    return binary->type_;
  }

  if (e1->type_ != e2->type_ || e1->indirection_ != e2->indirection_) {
    reporter_.ReportSemanticError("Invalid operands types in binary expression", e1, e2, binary->op_);
  } else if (binary->op_->GetType() != TokenType::T_MINUS) {
    reporter_.ReportSemanticError("Invalid binary operation for types", e1, e2, binary->op_);
  } else {
    binary->type_ = Type::LONG;
  }
  return binary->type_; // Type::NONE by default
}

Type TypeChecker::MatchPrimitives(ExprRef e1, ExprRef e2, bool to_left, ExprRef binary) {
  if (e1->type_ == e2->type_) return e1->type_;

  // FIXME: fix for structs
  if (!IsIntegerType(e1) || !IsIntegerType(e2)) {
    reporter_.ReportSemanticError("Invalid operands types in binary expression", e1, e2, binary->op_);
    return Type::NONE;
  }

  return to_left ? PromoteToLeft(e1, e2) : Promote(e1, e2);
}

Type TypeChecker::MatchMixed(ExprRef e1, ExprRef e2, bool to_left, ExprRef binary) {
  if (binary->op_->GetType() == TokenType::T_ASSIGN) {
    // FIXME: issue warnings on invalid assign conversions
    binary->type_ = e1->type_;
    binary->indirection_ = e1->indirection_;
  } else {
    if ((IsIntegerType(e1) && IsPointer(e2))) {
      binary->type_ = e2->type_;
      binary->indirection_ = e2->indirection_;
    } else if (IsIntegerType(e2) && IsPointer(e1)) {
      binary->type_ = e1->type_;
      binary->indirection_ = e1->indirection_;
    } else {
      reporter_.ReportSemanticError("Invalid types for pointer arithmetic in binary expression", e1, e2, binary->op_);
    }
  }
  return binary->type_;
}

Type TypeChecker::PromoteToLeft(ExprRef e1, ExprRef e2) {
  int t1 = (int) e1->type_, t2 = (int) e2->type_;

  if (t1 < t2) {
    reporter_.ReportSemanticError("Could not cast right expression", e1, e2, e1->op_);
    return Type::NONE;
  }

  return e1->type_;
}

Type TypeChecker::Promote(ExprRef e1, ExprRef e2) {
  int t1 = (int) e1->type_, t2 = (int) e2->type_;
  return (Type) std::max(t1, t2);
}

Type TypeChecker::Visit(const std::shared_ptr<Unary>& unary) {
  unary->right_->Accept(*this);

  switch (unary->op_->GetType()) {
    case TokenType::T_BIT_AND: {
      if (!unary->right_->IsLvalue()) {
        reporter_.Report("Expression to '&' operator must be an lvalue", unary->op_);
        return Type::NONE;
      }

      unary->indirection_ = unary->right_->indirection_ + 1;
      unary->type_ = unary->right_->type_;
      return unary->type_;
    }
    case TokenType::T_STAR: {
      if (!IsPointer(unary->right_)) {
        reporter_.Report("Expression to '*' must be a pointer", unary->op_);
        return Type::NONE;
      }

      unary->is_lvalue = true;
      unary->indirection_ = unary->right_->indirection_ - 1;
      unary->type_ = unary->right_->type_;
      return unary->type_;
    }
    default: {
      reporter_.Report("Invalid operator in 'unary' expression", unary->op_);
      return Type::NONE;
    }
  }
}

Type TypeChecker::Visit(const std::shared_ptr<Binary>& binary) {
  binary->left_->Accept(*this);
  binary->right_->Accept(*this);
  binary->type_ = MatchTypes(binary->left_, binary->right_, false, binary);
  return binary->type_;
}

Type TypeChecker::Visit(const std::shared_ptr<Assign>& assign) {
  assign->left_->Accept(*this);

  if (!assign->left_->IsLvalue()) {
    reporter_.Report("Assign to rvalue is not allowed", assign->op_);
    return Type::NONE;
  }

  // FIXME: add IsDeref() ?
  if (!assign->left_->IsVariable()) {
    std::static_pointer_cast<Unary>(assign->left_)->is_assign_ = true;
  }

  assign->right_->Accept(*this);
  assign->type_ = MatchTypes(assign->left_, assign->right_, true, assign);
  return assign->type_;
}

void TypeChecker::TypeCheck(const std::vector<std::shared_ptr<Stmt>>& stmts) {
  for (const auto& stmt : stmts) {
    stmt->Accept(*this);
  }
}

Type TypeChecker::Visit(const std::shared_ptr<Literal>& literal) {
  if (literal->IsVariable()) {
    int id = symbol_table_.Get(literal->op_->GetStringValue());

    if (id == -1) {
      reporter_.Report("Variable '"
          + literal->op_->GetStringValue() + "' has not been declared", literal->op_);
      return Type::NONE;
    }

    literal->type_ = symbol_table_.Get(id).type;
    literal->indirection_ = symbol_table_.Get(id).indirection;
    return literal->type_;
  } else {
    int val = literal->op_->GetIntValue();
    literal->type_ = (val <= 255 && val >= -256) ? Type::SHORT : Type::INT;
    return literal->type_;
  }
}

// #TODO: fix error reporting
Type TypeChecker::Visit(const std::shared_ptr<Print>& print) {
  print->expr_->Accept(*this);

  if (!IsIntegerType(print->expr_)) {
    reporter_.Report("'print' statement expects integer", print->token_);
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
    reporter_.Report("Used not-scalar where scalar is required", cond_stmt->token_);
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
    reporter_.Report("Used not-scalar where scalar is required", while_stmt->token_);
    return Type::NONE;
  }

  while_stmt->loop_block_->Accept(*this);
  return Type::NONE;
}

Type TypeChecker::Visit(const std::shared_ptr<For>& for_stmt) {
  for_stmt->init_->Accept(*this);

  if (for_stmt->condition_ != nullptr)
    for_stmt->condition_->Accept(*this);

  if (for_stmt->condition_ != nullptr && !IsIntegerType(for_stmt->condition_) && !IsPointer(for_stmt->condition_)) {
    reporter_.Report("Used not-scalar where scalar is required", for_stmt->token_);
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
    reporter_.Report("Variable '" + decl->name_->GetStringValue() + "' has unallowable type 'void'", decl->token_);
    return Type::NONE;
  }

  if (id == -1) {
    id = symbol_table_.Put(decl->name_->GetStringValue(), decl->var_type_, decl->indirection_);
  } else {
    reporter_.Report("Variable '" + decl->name_->GetStringValue() + "' has already been declared", decl->name_);
    return Type::NONE;
  }

  // FIXME: rewrite VarDecl to take literal as left parameter?
  if (decl->init_ != nullptr) {
    decl->init_->Accept(*this);
    if (!MatchTypeInit(decl->var_type_, decl->indirection_, decl->init_)) {
      reporter_.ReportSemanticError("The type of init expression does not match the declared type of the variable ",
          decl->var_type_, decl->indirection_, decl->init_, decl->token_);
    }
  }

  return Type::NONE;
}

} // namespace mcc
