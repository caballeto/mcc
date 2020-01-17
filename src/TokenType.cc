//
// Created by vladyslav on 15.01.20.
//

#include "common.h"
#include "TokenType.h"

namespace mcc {

std::ostream& operator<<(std::ostream& os, TokenType type) {
  switch (type) {
    case TokenType::T_PLUS: return os << "T_PLUS";
    case TokenType::T_MINUS: return os << "T_MINUS";
    case TokenType::T_STAR: return os << "T_STAR";
    case TokenType::T_SLASH: return os << "T_SLASH";
    case TokenType::T_INT_LITERAL: return os << "T_INT_LITERAL";
    case TokenType::T_EOF: return os << "T_EOF";
    case TokenType::T_SEMICOLON: return os << ";";;
    case TokenType::T_ASSIGN: return os << "=";
    case TokenType::T_IDENTIFIER: return os << "T_IDENTIFIER";
    case TokenType::T_PRINT: return os << "T_PRINT";
    case TokenType::T_INT: return os << "T_INT";
    case TokenType::T_LPAREN: return os << "T_LPAREN";
    case TokenType::T_RPAREN: return os << "T_RPAREN";
    case TokenType::T_LBRACE: return os << "T_LBRACE";
    case TokenType::T_RBRACE: return os << "T_RBRACE";
    case TokenType::T_EQUALS: return os << "T_EQUALS";
    case TokenType::T_NOT_EQUALS: return os << "T_NOT_EQUALS";
    case TokenType::T_LESS: return os << "T_LESS";
    case TokenType::T_GREATER: return os << "T_GREATER";
    case TokenType::T_LESS_EQUAL: return os << "T_LESS_EQUAL";
    case TokenType::T_GREATER_EQUAL: return os << "T_GREATER_EQUAL";
    case TokenType::T_IF: return os << "T_IF";
    case TokenType::T_ELSE: return os << "T_ELSE";
  }

  return os << static_cast<std::uint16_t>(type);
}

// #FIXME: the precedence values are relative and will change when new operators added
// #FIXME: see https://en.cppreference.com/w/c/language/operator_precedence
int GetPrecedence(TokenType type) {
  switch (type) {
    case TokenType::T_ASSIGN:
      return 8;
    case TokenType::T_EQUALS:
    case TokenType::T_NOT_EQUALS:
      return 9;
    case TokenType::T_GREATER:
    case TokenType::T_LESS:
    case TokenType::T_GREATER_EQUAL:
    case TokenType::T_LESS_EQUAL:
      return 10;
    case TokenType::T_PLUS:
    case TokenType::T_MINUS:
      return 11;
    case TokenType::T_STAR:
    case TokenType::T_SLASH:
      return 12;
    default:
      std::cerr << "Not a correct operator: " << type << std::endl;
      exit(1);
  }
}

} // namespace mcc
