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
  }

  return os << static_cast<std::uint16_t>(type);
}

int GetPrecedence(TokenType type) {
  switch (type) {
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
