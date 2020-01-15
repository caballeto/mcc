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

  void SetLine(int line) {
    line_ = line;
  }

  int GetLine() {
    return line_;
  }

  void SetIntValue(int value) {
    int_value_ = value;
  }

  int GetIntValue() {
    return int_value_;
  }

  void SetType(TokenType token_type) {
    token_type_ = token_type;
  }

  TokenType GetType() { // pass by value?
    return token_type_;
  }

 private:
  int line_;
  int int_value_;
  TokenType token_type_;
};

} // namespace mcc

#endif //MCC_SRC_TOKEN_H_
