//
// Created by vladyslav on 15.01.20.
//

#ifndef MCC_SRC_TOKENTYPE_H_
#define MCC_SRC_TOKENTYPE_H_

namespace mcc {

class Token;

enum class TokenType {
  // special
  T_EOF,
  T_LPAREN,
  T_RPAREN,
  T_LBRACE,
  T_RBRACE,
  T_COMMA,
  T_SEMICOLON,

  // operators
  T_PLUS,
  T_MINUS,
  T_STAR,
  T_SLASH,
  T_EQUALS,
  T_NOT_EQUALS,
  T_LESS,
  T_GREATER,
  T_LESS_EQUAL,
  T_GREATER_EQUAL,
  T_ASSIGN,
  T_BIT_AND,

  // literals
  T_INT_LITERAL,
  T_IDENTIFIER,

  // keywords
  T_PRINT,
  T_IF,
  T_ELSE,
  T_WHILE,
  T_DO,
  T_FOR,
  T_BREAK,
  T_CONTINUE,

  // types
  T_VOID = 100,
  T_SHORT = 101,
  T_INT = 102,
  T_LONG = 103
};

std::ostream& operator<<(std::ostream& os, TokenType type);
int GetPrecedence(const std::shared_ptr<Token>& op, bool is_unary);

} // namespace mcc

#endif //MCC_SRC_TOKENTYPE_H_
