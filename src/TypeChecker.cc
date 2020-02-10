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
  return expr->type_ == Type::INT || expr->type_ == Type::SHORT || expr->type_ == Type::LONG || expr->type_ == Type::CHAR;
}

bool TypeChecker::MatchTypeInit(Type type, int indirection, ExprRef init) {
  if (indirection == 0) {
    return IsPointer(init) ? true : (int) type >= (int) init->type_;
  } else {
    if (type != init->type_ || indirection != init->indirection_)
      reporter_.Warning("Assign of different type to pointer without a cast", type, indirection, init, init->op_);
    return true;
  }
}

// #TODO: build type hierarchy as a separate structure and provide API for type casting
// assuming types are not void
Type TypeChecker::MatchTypes(ExprRef e1, ExprRef e2, bool to_left, ExprRef binary) {
  if (e1->type_ == Type::VOID || e2->type_ == Type::VOID) {
    reporter_.Error("Invalid operands types in binary expression, 'void' is not allowed",
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
    reporter_.Error("Invalid operands types in binary expression", e1, e2, binary->op_);
  } else if (binary->op_->GetType() != TokenType::T_MINUS) {
    reporter_.Error("Invalid binary operation for types", e1, e2, binary->op_);
  } else {
    binary->type_ = Type::LONG; // pointer subtraction yields long
    binary->indirection_ = 0;
  }
  return binary->type_; // Type::NONE by default
}

Type TypeChecker::MatchPrimitives(ExprRef e1, ExprRef e2, bool to_left, ExprRef binary) {
  if (e1->type_ == e2->type_) return e1->type_;

  // FIXME: fix for structs
  if (!IsIntegerType(e1) || !IsIntegerType(e2)) {
    reporter_.Error("Invalid operands types in binary expression", e1, e2, binary->op_);
    return Type::NONE;
  }

  return to_left ? PromoteToLeft(e1, e2) : Promote(e1, e2);
}

Type TypeChecker::MatchMixed(ExprRef e1, ExprRef e2, bool to_left, ExprRef binary) {
  if (binary->op_->GetType() == TokenType::T_ASSIGN) { // assign
    binary->type_ = e1->type_;
    binary->indirection_ = e1->indirection_;
    reporter_.Warning("Assignment with pointer/integer without a cast", e1, e2, binary->op_);
  } else if (IsComparison(binary->op_->GetType())) { // comparison
    binary->type_ = Type::INT;
    binary->indirection_ = 0;
    reporter_.Warning("Comparison between pointer and integer", e1, e2, binary->op_);
  } else if (binary->op_->GetType() != TokenType::T_PLUS && binary->op_->GetType() != TokenType::T_MINUS) {
    reporter_.Error("Invalid operand for pointer arithmetic, only '+' and '-' allowed", e1, e2, binary->op_);
    return Type::NONE;
  } else if ((IsIntegerType(e1) && IsPointer(e2))) { // +, - for (int + pointer)
    if (binary->op_->GetType() == TokenType::T_MINUS) { // #TODO: check if can (x - ptr)
      reporter_.Error("Invalid operand for pointer arithmetic, only '+' and '-' allowed", e1, e2, binary->op_);
      return Type::NONE;
    }

    binary->type_ = e2->type_;
    binary->indirection_ = e2->indirection_;
    e1->to_scale_ = true;
  } else if (IsIntegerType(e2) && IsPointer(e1)) { // +, - for (pointer + int)
    binary->type_ = e1->type_;
    binary->indirection_ = e1->indirection_;
    e2->to_scale_ = true;
  } else {
    reporter_.Error("Invalid types for pointer arithmetic in binary expression", e1, e2, binary->op_);
  }

  return binary->type_;
}

Type TypeChecker::PromoteToLeft(ExprRef e1, ExprRef e2) {
  int t1 = (int) e1->type_, t2 = (int) e2->type_;

  if (t1 < t2) {
    reporter_.Error("Could not cast right expression", e1, e2, e1->op_);
    return Type::NONE;
  }

  return e1->type_;
}

Type TypeChecker::Promote(ExprRef e1, ExprRef e2) {
  int t1 = (int) e1->type_, t2 = (int) e2->type_;
  return (Type) std::max(t1, t2);
}

// #FIXME: add support for ++/-- operations + for pointers
Type TypeChecker::Visit(const std::shared_ptr<Unary>& unary) {
  unary->expr_->is_const_ = unary->is_const_;

  switch (unary->op_->GetType()) {
    case TokenType::T_PLUS:
    case TokenType::T_MINUS:
    case TokenType::T_NOT:
    case TokenType::T_NEG: {
      unary->expr_->Accept(*this);
      unary->indirection_ = unary->expr_->indirection_;
      unary->type_ = unary->expr_->type_;
      return unary->type_;
    }
    case TokenType::T_INC:
    case TokenType::T_DEC:
    case TokenType::T_BIT_AND: {
      unary->expr_->return_ptr_ = true;
      unary->expr_->Accept(*this);

      if (!unary->expr_->IsLvalue()) {
        reporter_.Report("Lvalue required for prefix operand", unary->op_);
        return Type::NONE;
      }

      // inc/dec on array is invalid
      if ((unary->op_->GetType() == TokenType::T_INC || unary->op_->GetType() == TokenType::T_DEC)
          && unary->expr_->is_indexable_) {
        reporter_.Report("Increment or decrement operations are invalid for arrays", unary->op_);
        return Type::NONE;
      }

      unary->to_scale_ = unary->expr_->indirection_ > 0; // scale for pointerss
      unary->indirection_ = unary->expr_->indirection_ + 1;
      unary->type_ = unary->expr_->type_;
      return unary->type_;
    }
    case TokenType::T_STAR: {
      unary->expr_->Accept(*this);

      if (!IsPointer(unary->expr_)) {
        reporter_.Report("Expression to '*' must be a pointer", unary->op_);
        return Type::NONE;
      }

      unary->is_lvalue_ = true;
      unary->indirection_ = unary->expr_->indirection_ - 1;
      unary->type_ = unary->expr_->type_;
      return unary->type_;
    }
    default: {
      reporter_.Report("Invalid operator in 'unary' expression", unary->op_);
      return Type::NONE;
    }
  }
}

Type TypeChecker::Visit(const std::shared_ptr<Binary>& binary) {
  binary->left_->is_const_ = binary->is_const_;
  binary->right_->is_const_ = binary->is_const_;
  binary->left_->Accept(*this);
  binary->right_->Accept(*this);
  binary->type_ = MatchTypes(binary->left_, binary->right_, false, binary);
  return binary->type_;
}

Type TypeChecker::Visit(const std::shared_ptr<Assign>& assign) {
  assign->left_->is_const_ = assign->is_const_;
  assign->right_->is_const_ = assign->is_const_;
  assign->left_->return_ptr_ = true;
  assign->left_->Accept(*this);

  if (!assign->left_->IsLvalue()) {
    reporter_.Report("Assign to rvalue is not allowed", assign->op_);
    return Type::NONE;
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
  if (literal->op_->GetType() == TokenType::T_IDENTIFIER) {
    if (literal->is_const_) {
      reporter_.Report("Variables initializers '"
          + literal->op_->GetStringValue() + "' must be constant", literal->op_);
      return Type::NONE;
    }

    int id = symbol_table_.Get(literal->op_->GetStringValue());

    if (id == -1) {
      reporter_.Report("Variable '"
          + literal->op_->GetStringValue() + "' has not been declared", literal->op_);
      return Type::NONE;
    }

    Entry descr = symbol_table_.Get(id);

    if (literal->is_function_ && !descr.is_function) {
      reporter_.Report("'" + literal->op_->GetStringValue() + "' is not a function", literal->op_);
      return Type::NONE;
    }

    if (literal->is_indexable_ && descr.array_len == 0 && descr.indirection == 0) {
      reporter_.Report("Invalid type for index operation, pointer or array expected", literal->op_);
      return Type::NONE;
    }

    literal->type_ = descr.type;
    literal->indirection_ = descr.indirection;
    literal->is_array = descr.array_len > 0;

    // usage of array as pointer, literal->is_indexable_ is false, set to true
    // so usage *(array + 5) = 10, will return pointer to array
    if (!literal->is_indexable_ && descr.array_len > 0) {
      literal->is_indexable_ = true;
      literal->indirection_++;
    }

    return literal->type_;
  } else if (literal->op_->GetType() == TokenType::T_STR_LIT) {
    literal->type_ = Type::CHAR;
    literal->indirection_ = 1;
    return literal->type_;
  } else { // integer
    int val = literal->op_->GetIntValue();
    literal->type_ = (val <= 255 && val >= -256) ? Type::CHAR : Type::INT;
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

Type TypeChecker::Visit(const std::shared_ptr<Return>& return_stmt) {
  return_stmt->expr_->Accept(*this);

  if (curr_func->return_type_ == Type::VOID) {
    reporter_.Error("Declared return type is 'void', but return statement found",
                    curr_func->return_type_, curr_func->indirection_, return_stmt->expr_, return_stmt->token_);
    return Type::NONE;
  }

  if (!MatchTypeInit(curr_func->return_type_, curr_func->indirection_, return_stmt->expr_)) {
    reporter_.Error("Declared type of init expression does not correspond to inferred type ",
                    curr_func->return_type_, curr_func->indirection_, return_stmt->expr_, return_stmt->token_);
    return Type::NONE;
  }

  return Type::NONE;
}

Type TypeChecker::Visit(const std::shared_ptr<FuncDecl>& func_decl) {
  int id = symbol_table_.Get(func_decl->name_->GetStringValue());

  if (id == -1) {
    symbol_table_.Put(func_decl);
  } else {
    reporter_.Report("Function '" + func_decl->name_->GetStringValue() + "' redefinition", func_decl->name_);
    return Type::NONE;
  }

  curr_func = func_decl;
  func_decl->body_->Accept(*this);
  curr_func = nullptr;
  return Type::NONE;
}

// int array[25];
Type TypeChecker::Visit(const std::shared_ptr<VarDecl>& decl) {
  int id = symbol_table_.Get(decl->name_->GetStringValue());

  if (decl->var_type_ == Type::VOID && decl->indirection_ == 0) {
    reporter_.Report("Variable '" + decl->name_->GetStringValue() + "' has unallowable type 'void'", decl->token_);
    return Type::NONE;
  }

  if (id == -1) {
    symbol_table_.Put(decl->name_->GetStringValue(), decl->var_type_, decl->indirection_, decl->array_len_, false);
  } else {
    reporter_.Report("Variable '" + decl->name_->GetStringValue() + "' has already been declared", decl->name_);
    return Type::NONE;
  }

  if (decl->init_ != nullptr) {
    decl->init_->is_const_ = decl->is_const_init_;
    decl->init_->Accept(*this);
    if (!MatchTypeInit(decl->var_type_, decl->indirection_, decl->init_)) {
      reporter_.Error("The type of init expression does not match the declared type of the variable ",
                      decl->var_type_, decl->indirection_, decl->init_, decl->token_);
    }
  }

  return Type::NONE;
}

Type TypeChecker::Visit(const std::shared_ptr<Call>& call) {
  call->name_->is_function_ = true;
  call->name_->Accept(*this);
  call->type_ = call->name_->type_;
  call->indirection_ = call->name_->indirection_;
  // #TODO: check for number of parameters + types
  if (!call->args_->expr_list_.empty()) {
    call->args_->expr_list_[0]->Accept(*this);
  }

  return call->type_;
}

Type TypeChecker::Visit(const std::shared_ptr<Index>& index) {
  index->name_->is_indexable_ = true;
  index->name_->Accept(*this);
  index->index_->Accept(*this);
  index->index_->to_scale_ = true;
  index->is_lvalue_ = true;

  if (!IsIntegerType(index->index_)) { // #FIXME: could pointers be used as index?
    reporter_.Report("Index must be an integer type", index->name_->op_);
    return Type::NONE;
  }

  index->type_ = index->name_->type_;
  index->indirection_ = std::max(index->name_->indirection_ - 1, 0); // FIXME: for pointers
  return index->type_;
}

Type TypeChecker::Visit(const std::shared_ptr<Grouping>& grouping) {
  grouping->expr_->is_const_ = grouping->is_const_;
  grouping->expr_->return_ptr_ = grouping->return_ptr_;

  grouping->expr_->Accept(*this);

  grouping->is_lvalue_ = grouping->expr_->is_lvalue_;
  grouping->type_ = grouping->expr_->type_;
  grouping->indirection_ = grouping->expr_->indirection_;
  return grouping->type_;
}

Type TypeChecker::Visit(const std::shared_ptr<Ternary>& ternary) {
  ternary->condition_->is_const_ = ternary->is_const_;
  ternary->then_->is_const_ = ternary->is_const_;
  ternary->else_->is_const_ = ternary->is_const_;
  ternary->condition_->Accept(*this);
  ternary->then_->Accept(*this);
  ternary->else_->Accept(*this);
  ternary->type_ = MatchTypes(ternary->then_, ternary->else_, false, ternary);
  return ternary->type_;
}

Type TypeChecker::Visit(const std::shared_ptr<Postfix>& postfix) {
  postfix->expr_->is_const_ = postfix->is_const_; // #FIXME: throw error immediately, as postfix is lvalue?
  postfix->expr_->return_ptr_ = true;
  postfix->expr_->Accept(*this);

  if (!postfix->expr_->IsLvalue()) {
    reporter_.Report("Lvalue required for postfix operand", postfix->op_);
    return Type::NONE;
  }

  // inc/dec on array is invalid
  if ((postfix->op_->GetType() == TokenType::T_INC || postfix->op_->GetType() == TokenType::T_DEC)
      && postfix->expr_->is_indexable_) {
    reporter_.Report("Increment or decrement operations are invalid for arrays", postfix->op_);
    return Type::NONE;
  }

  postfix->to_scale_ = postfix->expr_->indirection_ > 0; // scale for pointer
  postfix->is_lvalue_ = false;
  postfix->type_ = postfix->expr_->type_;
  postfix->indirection_ = postfix->expr_->indirection_;
  return Type::NONE;
}
bool TypeChecker::IsComparison(TokenType type) {
  return type == TokenType::T_EQUALS || type == TokenType::T_NOT_EQUALS
    || type == TokenType::T_LESS || type == TokenType::T_LESS_EQUAL
    || type == TokenType::T_GREATER || type == TokenType::T_GREATER_EQUAL;
}

} // namespace mcc
