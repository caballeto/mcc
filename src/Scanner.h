//
// Created by vladyslav on 15.01.20.
//

#ifndef MCC_SRC_SCANNER_H_
#define MCC_SRC_SCANNER_H_

#include "common.h"
#include "Token.h"
#include "ErrorReporter.h"

namespace mcc {

class Scanner {
 public:
  Scanner(const std::string& input_file, ErrorReporter& reporter);
  ~Scanner();

  std::shared_ptr<Token> GetToken();

 private:
  char Next();
  char NextCharSkipSpaces();
  int ScanInt(int int_value);
  void Putback(char c);
  std::string ScanIdent(char c);
  bool IsKeyword(const std::string& identifier);
  TokenType GetKeywordToken(const std::string& keyword);

  ErrorReporter& reporter_;
  std::map<std::string, TokenType> keywords_;
  std::ifstream source_stream_;
  int line_;
  int c_ = 0;
  char putback_ = '\0';
};

} // namespace mcc

#endif //MCC_SRC_SCANNER_H_
