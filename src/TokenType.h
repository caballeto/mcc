//
// Created by vladyslav on 15.01.20.
//

#ifndef MCC_SRC_TOKENTYPE_H_
#define MCC_SRC_TOKENTYPE_H_

namespace mcc {

enum class TokenType {
  // special
  T_EOF,

  // operators
  T_PLUS,
  T_MINUS,
  T_STAR,
  T_SLASH,
  T_SEMICOLON,
  T_EQUALS,
  T_NOT_EQUALS,
  T_LESS,
  T_GREATER,
  T_LESS_EQUAL,
  T_GREATER_EQUAL,
  T_ASSIGN,

  // literals
  T_INT_LITERAL,
  T_IDENTIFIER,

  // keywords
  T_PRINT,
  T_INT
};

std::ostream& operator<<(std::ostream& os, TokenType type);
int GetPrecedence(TokenType type);

} // namespace mcc

#endif //MCC_SRC_TOKENTYPE_H_
