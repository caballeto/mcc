//
// Created by vladyslav on 15.01.20.
//

#include "Parser.h"

namespace mcc {

std::shared_ptr<Expr> Parser::Expression(int precedence) {
  std::shared_ptr<Expr> left = Primary(), right;
  std::shared_ptr<Token> curr_op = Peek();

  if (curr_op->GetType() == TokenType::T_EOF)
    return left;

  while (precedence < GetPrecedence(curr_op->GetType())) {
    Next();
    right = Expression(GetPrecedence(curr_op->GetType()));
    left = std::make_shared<Binary>(curr_op, left, right); // Do I need to pass whole Token?
    curr_op = Peek();
    if (curr_op->GetType() == TokenType::T_EOF) {
      return left;
    }
  }

  return left;
}

std::shared_ptr<Expr> Parser::Primary() {
  switch (Peek()->GetType()) {
    case TokenType::T_INT_LITERAL: {
      std::shared_ptr<Expr> literal = std::make_shared<Literal>(Peek());
      Next();
      return literal;
    }
    default:
      std::cerr << " Error on line " << Peek()->GetLine() <<
        " Primary expects int literal." << std::endl;
      exit(1);
  }
}

std::shared_ptr<Token> Parser::Next() {
  token_ = scanner_.GetToken();
  return token_;
}

bool Parser::Match(TokenType type) {
  if (token_->GetType() == type) {
    Next();
    return true;
  } else {
    return false;
  }
}

std::shared_ptr<Token> Parser::Peek() {
  return token_;
}

} // namespace mcc
