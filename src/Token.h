//
// Created by vladyslav on 15.01.20.
//

#ifndef MCC_SRC_TOKEN_H_
#define MCC_SRC_TOKEN_H_

#include "common.h"
#include "TokenType.h"

namespace mcc {

class Token {
 public:

  inline void SetString(std::string value) {
    identifier_ = std::move(value);
  }

  inline std::string& String() {
    return identifier_;
  }

  inline void SetCount(int c) {
    c_ = c;
  }

  inline int GetCount() {
    return c_;
  }

  inline void SetLine(int line) {
    line_ = line;
  }

  inline int GetLine() {
    return line_;
  }

  inline void SetInt(int value) {
    int_value_ = value;
  }

  inline int Int() {
    return int_value_;
  }

  inline void SetType(const TokenType& token_type) {
    token_type_ = token_type;
  }

  inline TokenType GetType() {
    return token_type_;
  }

 private:
  int line_;
  int int_value_;
  int c_; // character count in line
  TokenType token_type_;
  std::string identifier_;
};

} // namespace mcc

#endif //MCC_SRC_TOKEN_H_
