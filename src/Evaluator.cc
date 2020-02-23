//
// Created by vladyslav on 23.02.20.
//

#include "Evaluator.h"

namespace mcc {

long Evaluator::Visit(TypeCast &type_cast) {
  return 0;
}

long Evaluator::Visit(Typedef &typedef_stmt) {
  return 0;
}

long Evaluator::Visit(Switch &switch_stmt) {
  return 0;
}

long Evaluator::Visit(Enum &enum_decl) {
  return 0;
}

long Evaluator::Visit(Union &union_decl) {
  return 0;
}

long Evaluator::Visit(Struct &struct_decl) {
  return 0;
}

long Evaluator::Visit(Label &label) {
  return 0;
}

long Evaluator::Visit(GoTo &go_to) {
  return 0;
}

long Evaluator::Visit(Access &access) {
  return 0;
}

long Evaluator::Visit(Index &index) {
  return 0;
}

long Evaluator::Visit(Call &call) {
  return 0;
}

long Evaluator::Visit(Ternary &ternary) {
  long cond = ternary.condition_->Accept(*this);
  long left = ternary.then_->Accept(*this);
  long right = ternary.else_->Accept(*this);
  return cond ? left : right;
}

long Evaluator::Visit(Postfix &postfix) {
  return 0;
}

long Evaluator::Visit(Grouping &grouping) {
  return grouping.expr_->Accept(*this);
}

long Evaluator::Visit(Binary &binary) {
  long left = binary.left_->Accept(*this);
  long right = binary.right_->Accept(*this);

  switch (binary.op_->GetType()) {
    case TokenType::T_PLUS:
      return left + right;
    case TokenType::T_MINUS:
      return left - right;
    case TokenType::T_STAR:
      return left * right;
    case TokenType::T_SLASH:
      return left / right;
    case TokenType::T_OR:
    case TokenType::T_AND:
      std::cerr << "Not implemented error '&&', '||' operators" << std::endl;
      exit(1);
    case TokenType::T_BIT_AND:
      return left & right;
    case TokenType::T_BIT_OR:
      return left | right;
    case TokenType::T_BIT_XOR:
      return left ^ right;
    case TokenType::T_LSHIFT:
      return left << right;
    case TokenType::T_RSHIFT:
      return left >> right;
    case TokenType::T_EQUALS:
      return left == right;
    case TokenType::T_NOT_EQUALS:
      return left != right;
    case TokenType::T_GREATER:
      return left > right;
    case TokenType::T_LESS:
      return left < right;
    case TokenType::T_GREATER_EQUAL:
      return left >= right;
    case TokenType::T_LESS_EQUAL:
      return left <= right;
    default: {
      std::cerr << "Internal error: undefined binary operator" << binary.op_->GetType() << std::endl;
      exit(1);
    }
  }
}

long Evaluator::Visit(Unary &unary) {
  long value = unary.expr_->Accept(*this);

  switch (unary.op_->GetType()) {
    case TokenType::T_PLUS: return value;
    case TokenType::T_MINUS: return -value;
    case TokenType::T_NEG: return ~value;
    case TokenType::T_NOT: return !value;
    case TokenType::T_SIZEOF: return unary.op_->Int();
    default: {
      std::cerr << "Invalid operator in unary const initializer: " << unary.op_ << std::endl;
      exit(1);
    }
  }

  return 0;
}

long Evaluator::Visit(Literal &literal) {
  return literal.op_->Int(); // could only be INT_LITERAL
}

long Evaluator::Visit(Assign &assign) {
  return 0;
}

long Evaluator::Visit(FuncDecl &func_decl) {
  return 0;
}

long Evaluator::Visit(VarDecl &var_decl) {
  return 0;
}

long Evaluator::Visit(Print &print) {
  return 0;
}

long Evaluator::Visit(ExpressionStmt &expr_stmt) {
  return 0;
}

long Evaluator::Visit(Conditional &cond_stmt) {
  return 0;
}

long Evaluator::Visit(Block &block_stmt) {
  return 0;
}

long Evaluator::Visit(While &while_stmt) {
  return 0;
}

long Evaluator::Visit(For &for_stmt) {
  return 0;
}

long Evaluator::Visit(DeclList &decl_list) {
  return 0;
}

long Evaluator::Visit(ExprList &expr_list) {
  return 0;
}

long Evaluator::Visit(ControlFlow &flow_stmt) {
  return 0;
}

long Evaluator::Visit(Return &return_stmt) {
  return 0;
}

} // namespace mcc