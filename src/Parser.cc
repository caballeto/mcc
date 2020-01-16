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
        Consume(TokenType::T_SEMICOLON, "Expected ';' after 'print' statement.");
        stmts.push_back(std::make_shared<Print>(expr));
        break;
      }
      case TokenType::T_INT: {
        Match(TokenType::T_INT);
        std::shared_ptr<Token> token = Consume(TokenType::T_IDENTIFIER,"Expected identifier after type declaration.");
        symbol_table_.Put(token->GetStringValue());
        Consume(TokenType::T_SEMICOLON, "Expected ';' after 'int' declaration.");
        stmts.push_back(std::make_shared<VarDecl>(token->GetStringValue()));
        break;
      }
      case TokenType::T_IDENTIFIER: {
        std::shared_ptr<Literal> left = std::make_shared<Literal>(Peek());

        if (!symbol_table_.Contains(left->literal_->GetStringValue())) {
          std::cerr << "Variable '" << left->literal_->GetStringValue() << "' has not been declared." << std::endl;
          exit(1); // #TODO: Add parser synchronization; check type compatibility here
        }

        Next();
        Consume(TokenType::T_EQUAL, "Expected '=' in assign expression.");
        std::shared_ptr<Expr> right = Expression(0);
        Consume(TokenType::T_SEMICOLON, "Expected ';' after expression statement.");
        std::shared_ptr<Assign> assign = std::make_shared<Assign>(left, right);
        stmts.push_back(std::make_shared<ExpressionStmt>(assign));
        break;
      }
      default:
        std::cerr << "Invalid statement-level token" << std::endl;
        exit(1);
    }
  }

  return std::move(stmts);
}

// Read about named local return value optimization and copy-elision
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
    case TokenType::T_IDENTIFIER: {
      std::shared_ptr<Literal> literal = std::make_shared<Literal>(Peek());

      if (!symbol_table_.Contains(literal->literal_->GetStringValue())) {
        std::cerr << "Variable '" << literal->literal_->GetStringValue() << "' has not been declared." << std::endl;
        exit(1); // #TODO: Add parser synchronization; check type compatibility here
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

} // namespace mcc
