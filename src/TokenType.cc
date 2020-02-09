//
// Created by vladyslav on 15.01.20.
//

#include "common.h"
#include "TokenType.h"
#include "Token.h"
#include "ParseException.h"

namespace mcc {

std::ostream& operator<<(std::ostream& os, TokenType type) {
  switch (type) {
    case TokenType::T_PLUS: return os << "'+'";
    case TokenType::T_MINUS: return os << "'-'";
    case TokenType::T_STAR: return os << "'*'";
    case TokenType::T_SLASH: return os << "'/'";
    case TokenType::T_INT_LITERAL: return os << "INT_LITERAL";
    case TokenType::T_EOF: return os << "EOF";
    case TokenType::T_SEMICOLON: return os << "';'";;
    case TokenType::T_ASSIGN: return os << "'='";
    case TokenType::T_IDENTIFIER: return os << "IDENTIFIER";
    case TokenType::T_PRINT: return os << "'print'";
    case TokenType::T_INT: return os << "'int'";
    case TokenType::T_LPAREN: return os << "'('";
    case TokenType::T_RPAREN: return os << "')'";
    case TokenType::T_LBRACE: return os << "'{'";
    case TokenType::T_RBRACE: return os << "'}'";
    case TokenType::T_EQUALS: return os << "'=='";
    case TokenType::T_NOT_EQUALS: return os << "'!='";
    case TokenType::T_LESS: return os << "'<'";
    case TokenType::T_GREATER: return os << "'>'";
    case TokenType::T_LESS_EQUAL: return os << "'<='";
    case TokenType::T_GREATER_EQUAL: return os << "'>='";
    case TokenType::T_IF: return os << "'if'";
    case TokenType::T_ELSE: return os << "'else'";
    case TokenType::T_COMMA: return os << "','";
    case TokenType::T_WHILE: return os << "'while'";
    case TokenType::T_DO: return os << "'do'";
    case TokenType::T_FOR: return os << "'for'";
    case TokenType::T_BREAK: return os << "'break'";
    case TokenType::T_CONTINUE: return os << "'continue'";
    case TokenType::T_VOID: return os << "'void'";
    case TokenType::T_SHORT: return os << "'short'";
    case TokenType::T_LONG: return os << "'long'";
    case TokenType::T_BIT_AND: return os << "'&'";
    case TokenType::T_RETURN: return os << "'return'";
    case TokenType::T_INC: return os << "'++'";
    case TokenType::T_DEC: return os << "'--'";
    case TokenType::T_NOT: return os << "'!'";
    case TokenType::T_NEG: return os << "'~'";
    case TokenType::T_DOT: return os << "'.'";
    case TokenType::T_ARROW: return os << "'->'";
    case TokenType::T_LBRACKET: return os << "'['";
    case TokenType::T_RBRACKET: return os << "']'";
    case TokenType::T_MOD: return os << "'%'";
    case TokenType::T_BIT_OR: return os << "'|'";
    case TokenType::T_BIT_XOR: return os << "'^'";
    case TokenType::T_QUESTION: return os << "'?'";
    case TokenType::T_COLON: return os << "':'";
    case TokenType::T_OR: return os << "'||'";
    case TokenType::T_AND: return os << "'&&'";
    case TokenType::T_LSHIFT: return os << "'<<'";
    case TokenType::T_RSHIFT: return os << "'>>'";
  }

  return os << static_cast<std::uint16_t>(type);
}

// #FIXME: the precedence values are relative and will change when new operators added
// #FIXME: see https://en.cppreference.com/w/c/language/operator_precedence
int GetPrecedence(const std::shared_ptr<Token>& op, bool is_unary) {
  switch (op->GetType()) {
    case TokenType::T_ASSIGN:
      return 2;
    case TokenType::T_QUESTION:
      return 3;
    case TokenType::T_OR:
      return 4;
    case TokenType::T_AND:
      return 5;
    case TokenType::T_BIT_OR:
      return 6;
    case TokenType::T_BIT_XOR:
      return 7;
    case TokenType::T_BIT_AND:
      return is_unary ? 14 : 8;
    case TokenType::T_EQUALS:
    case TokenType::T_NOT_EQUALS:
      return 9;
    case TokenType::T_GREATER:
    case TokenType::T_LESS:
    case TokenType::T_GREATER_EQUAL:
    case TokenType::T_LESS_EQUAL:
      return 10;
    case TokenType::T_LSHIFT:
    case TokenType::T_RSHIFT:
      return 11;
    case TokenType::T_PLUS:
    case TokenType::T_MINUS:
      return 12;
    case TokenType::T_STAR:
      return is_unary ? 14 : 13;
    case TokenType::T_MOD:
    case TokenType::T_SLASH:
      return 13;
    case TokenType::T_NOT:
    case TokenType::T_NEG:
    case TokenType::T_DEC:
    case TokenType::T_INC:
      return 14;
    case TokenType::T_LPAREN:
    case TokenType::T_LBRACKET:
      return 15;
    default:
      throw ParseException("Expected operator", op);
  }
}

} // namespace mcc
