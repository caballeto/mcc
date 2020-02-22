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
  T_NOT,
  T_NEG,
  T_DOT,
  T_ARROW,
  T_LBRACKET,
  T_RBRACKET,
  T_PLUS,
  T_MINUS,
  T_STAR,
  T_SLASH,
  T_MOD,
  T_EQUALS,
  T_NOT_EQUALS,
  T_LESS,
  T_GREATER,
  T_LESS_EQUAL,
  T_GREATER_EQUAL,
  T_ASSIGN,
  T_BIT_AND,
  T_BIT_OR,
  T_BIT_XOR,
  T_INC,
  T_DEC,
  T_QUESTION,
  T_COLON,
  T_OR,
  T_AND,
  T_LSHIFT,
  T_RSHIFT,

  // literals
  T_INT_LIT,
  T_STR_LIT,
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
  T_RETURN,
  T_GOTO,
  T_STRUCT,
  T_UNION,
  T_ENUM,
  T_TYPEDEF,
  T_SWITCH,
  T_CASE,
  T_DEFAULT,
  T_SIZEOF,
  T_EXTERN,
  T_STATIC,
  T_TAG,

  // type
  T_NONE,
  T_VOID = 100,
  T_CHAR = 101,
  T_SHORT = 102,
  T_INT = 103,
  T_LONG = 104
};

std::ostream& operator<<(std::ostream& os, TokenType type);
int GetPrecedence(const std::shared_ptr<Token>& op, bool is_unary);

} // namespace mcc

#endif //MCC_SRC_TOKENTYPE_H_
