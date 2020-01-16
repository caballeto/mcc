//
// Created by vladyslav on 15.01.20.
//

#ifndef MCC_SRC_TOKEN_H_
#define MCC_SRC_TOKEN_H_

#include <utility>

#include "common.h"
#include "TokenType.h"

namespace mcc {

class Token {
 public:

  inline void SetStringValue(std::string value) {
    identifier_ = std::move(value);
  }

  inline std::string& GetStringValue() {
    return identifier_;
  }

  inline void SetLine(int line) {
    line_ = line;
  }

  inline int GetLine() {
    return line_;
  }

  inline void SetIntValue(int value) {
    int_value_ = value;
  }

  inline int GetIntValue() {
    return int_value_;
  }

  inline void SetType(TokenType token_type) {
    token_type_ = token_type;
  }

  inline TokenType GetType() { // pass by value?
    return token_type_;
  }

 private:
  int line_;
  int int_value_;
  TokenType token_type_;
  std::string identifier_;
};

} // namespace mcc

#endif //MCC_SRC_TOKEN_H_
