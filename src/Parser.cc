//
// Created by vladyslav on 15.01.20.
//

#include "Parser.h"

namespace mcc {

std::vector<std::shared_ptr<Stmt>> Parser::Parse() {
  std::vector<std::shared_ptr<Stmt>> stmts;

  while (Peek()->GetType() != TokenType::T_EOF) {
    switch (Peek()->GetType()) {
      case TokenType::T_PRINT: {
        Match(TokenType::T_PRINT);
        std::shared_ptr<Expr> expr = Expression(0);
        Consume(TokenType::T_SEMICOLON, "Expected ';' after Print statement.");
        stmts.push_back(std::make_shared<Print>(expr));
        break;
      }
      default:
        std::cerr << "Invalid statement-level token" << std::endl;
        exit(1);
    }
  }

  return std::move(stmts);
}

std::shared_ptr<Expr> Parser::Expression(int precedence) {
  std::shared_ptr<Expr> left = Primary(), right;
  std::shared_ptr<Token> curr_op = Peek();

  if (curr_op->GetType() == TokenType::T_SEMICOLON)
    return left;

  while (precedence < GetPrecedence(curr_op->GetType())) {
    Next();
    right = Expression(GetPrecedence(curr_op->GetType()));
    left = std::make_shared<Binary>(curr_op, left, right); // Do I need to pass whole Token?
    curr_op = Peek();
    if (curr_op->GetType() == TokenType::T_SEMICOLON) {
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

void Parser::Consume(TokenType type, const std::string& message) {
  if (Peek()->GetType() == type) {
    Next();
  } else {
    std::cerr << message << std::endl;
    exit(1);
  }
}

} // namespace mcc
