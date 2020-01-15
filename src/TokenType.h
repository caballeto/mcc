//
// Created by vladyslav on 15.01.20.
//

#ifndef MCC_SRC_TOKENTYPE_H_
#define MCC_SRC_TOKENTYPE_H_

namespace mcc {

enum class TokenType {
  T_PLUS,
  T_MINUS,
  T_STAR,
  T_SLASH,
  T_EOF,

  T_INT_LITERAL
};

std::ostream& operator<<(std::ostream& os, TokenType type);
int GetPrecedence(TokenType type);

} // namespace mcc

#endif //MCC_SRC_TOKENTYPE_H_
