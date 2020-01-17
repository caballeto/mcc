//
// Created by vladyslav on 15.01.20.
//

#include "Parser.h"

namespace mcc {

std::vector<std::shared_ptr<Stmt>> Parser::Parse() {
  std::vector<std::shared_ptr<Stmt>> stmts;

  while (Peek()->GetType() != TokenType::T_EOF) {
    stmts.push_back(Statement());
  }

  return std::move(stmts);
}

std::shared_ptr<Stmt> Parser::Statement() {
  switch (Peek()->GetType()) {
    case TokenType::T_PRINT: {
      Match(TokenType::T_PRINT);
      std::shared_ptr<Expr> expr = Expression(0);
      Consume(TokenType::T_SEMICOLON, "Expected ';' after 'print' Statement.");
      return std::make_shared<Print>(expr);
    }
    case TokenType::T_INT: {
      Match(TokenType::T_INT);
      std::shared_ptr<Token> token = Consume(TokenType::T_IDENTIFIER,"Expected identifier after type declaration.");
      symbol_table_.Put(token->GetStringValue());
      Consume(TokenType::T_SEMICOLON, "Expected ';' after 'int' declaration.");
      return std::make_shared<VarDecl>(token->GetStringValue());
    }
    case TokenType::T_IF: {
      Match(TokenType::T_IF);
      Consume(TokenType::T_LPAREN, "Expected '(' after 'if'.");
      std::shared_ptr<Expr> condition = Expression(0);
      Consume(TokenType::T_RPAREN, "Expected ')' after if condition.");
      std::shared_ptr<Block> then_block_ = BlockStatement(), else_block_;

      if (Match(TokenType::T_ELSE))
        else_block_ = BlockStatement();

      return std::make_shared<Conditional>(condition, then_block_, else_block_);
    }
    case TokenType::T_LBRACE: {
      return BlockStatement();
    }
    default:
      return ExpressionStatement();
  }
}

std::shared_ptr<Stmt> Parser::ExpressionStatement() {
  switch (Peek()->GetType()) {
    case TokenType::T_IDENTIFIER: {
      std::shared_ptr<Expr> expr = Expression(0);
      Consume(TokenType::T_SEMICOLON, "Expected ';' after expression Statement.");
      return std::make_shared<ExpressionStmt>(expr);
    }
    default:
      std::cerr << "Only assign expressions are supported in top-level expression statements." << std::endl;
      exit(1);
  }
}
/*
std::shared_ptr<Expr> Parser::AssignExpression() {
  std::shared_ptr<Literal> left = std::make_shared<Literal>(Peek());

  if (!symbol_table_.Contains(left->literal_->GetStringValue())) {
    std::cerr << "Variable '" << left->literal_->GetStringValue() << "' has not been declared." << std::endl;
    exit(1); // #TODO: Add parser synchronization; check type compatibility here
  }

  Next();
  Consume(TokenType::T_ASSIGN, "Expected '=' in assign expression.");
  std::shared_ptr<Expr> right = Expression(0);
  return std::make_shared<Assign>(left, right);
}*/

// Read about named local return value optimization and copy-elision
std::shared_ptr<Expr> Parser::Expression(int precedence) {
  std::shared_ptr<Expr> left = Primary(), right;
  std::shared_ptr<Token> curr_op = Peek();

  if (curr_op->GetType() == TokenType::T_SEMICOLON || curr_op->GetType() == TokenType::T_RPAREN)
    return left;

  while (precedence < GetPrecedence(curr_op->GetType())) {
    switch (curr_op->GetType()) {
      case TokenType::T_ASSIGN: {
        if (!left->IsVariable()) {
          std::cerr << "Only varibles are allowed on left side of assign expression." << std::endl;
          exit(1);
        }

        Next();
        right = Expression(GetPrecedence(curr_op->GetType()) - 1);
        left = std::make_shared<Assign>(left, right);
        break;
      }
      default: {
        Next();
        right = Expression(GetPrecedence(curr_op->GetType()));
        left = std::make_shared<Binary>(curr_op, left, right);
      }
    }

    curr_op = Peek();
    if (curr_op->GetType() == TokenType::T_SEMICOLON || curr_op->GetType() == TokenType::T_RPAREN) {
      return left;
    }
  }

  return left;
}

std::shared_ptr<Expr> Parser::Primary() {
  switch (Peek()->GetType()) {
    case TokenType::T_IDENTIFIER: {
      std::shared_ptr<Literal> literal = std::make_shared<Literal>(Peek());

      if (!symbol_table_.Contains(literal->literal_->GetStringValue())) {
        std::cerr << "Variable '" << literal->literal_->GetStringValue() << "' has not been declared." << std::endl;
        exit(1);
      }

      Next();
      return literal;
    }
    case TokenType::T_INT_LITERAL: {
      std::shared_ptr<Literal> literal = std::make_shared<Literal>(Peek());
      Next();
      return literal;
    }
    default:
      std::cerr << " Error on line " << Peek()->GetLine() <<
        " Primary expects int or identifier literal." << std::endl;
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

std::shared_ptr<Token> Parser::Consume(TokenType type, const std::string& message) {
  if (Peek()->GetType() == type) {
    std::shared_ptr<Token> token = Peek();
    Next();
    return token;
  } else {
    std::cerr << message << std::endl;
    exit(1);
  }
}

std::shared_ptr<Block> Parser::BlockStatement() {
  Consume(TokenType::T_LBRACE, "Expected '{' at start of block.");
  std::vector<std::shared_ptr<Stmt>> stmts;

  while (Peek()->GetType() != TokenType::T_RBRACE) {
    stmts.push_back(Statement());
  }

  Match(TokenType::T_RBRACE);

  return std::make_shared<Block>(std::move(stmts));
}

} // namespace mcc
