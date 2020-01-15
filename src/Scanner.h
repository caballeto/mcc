//
// Created by vladyslav on 15.01.20.
//

#ifndef MCC_SRC_SCANNER_H_
#define MCC_SRC_SCANNER_H_

#include "common.h"
#include "Token.h"

namespace mcc {

class Scanner {
 public:
  explicit Scanner(const std::string& input_file);
  ~Scanner();

  std::shared_ptr<Token> GetToken();

 private:
  char Next();
  char NextCharSkipSpaces();
  int ScanInt(int int_value);
  void Putback(char c);

  std::ifstream source_stream_;
  int line_;
  char putback_ = '\0';
};

} // namespace mcc

#endif //MCC_SRC_SCANNER_H_
