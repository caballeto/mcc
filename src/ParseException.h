//
// Created by vladyslav on 05.02.20.
//

#ifndef MCC_SRC_PARSEEXCEPTION_H_
#define MCC_SRC_PARSEEXCEPTION_H_

#include "common.h"
#include "Token.h"

namespace mcc {

class ParseException : std::exception {
 public:
  ParseException(std::string message, std::shared_ptr<Token> token)
    : message_(std::move(message)), token_(std::move(token))
  { }

  inline const std::string& GetMessage() const { return message_; }
  inline const std::shared_ptr<Token>& GetToken() const { return  token_; }

 private:
  std::string message_;
  std::shared_ptr<Token> token_;
};

} // namespace mcc

#endif //MCC_SRC_PARSEEXCEPTION_H_
