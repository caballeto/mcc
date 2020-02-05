//
// Created by vladyslav on 03.02.20.
//

#ifndef MCC_SRC_ERRORREPORTER_H_
#define MCC_SRC_ERRORREPORTER_H_

#include "common.h"
#include "Type.h"
#include "Token.h"
#include "ParseException.h"

#define RED "\033[0;31m"
#define RESET "\033[0m"

namespace mcc {

class ErrorReporter {
 public:
  explicit ErrorReporter(std::ostream& os)
    : os_(os), errors_(0)
  { }

  void SetFile(const std::string& input_file) {
    input_file_ = input_file;
  }

  void PrintErrorLine(const std::string& file, int line, int c);
  void ReportParseError(const ParseException& exception);
  void ReportSemanticError(const std::string& message, Type t1, Type t2, const std::shared_ptr<Token>& token);

  void Report(const std::string& message);
  void Report(const std::string& message, char c, int line);
  void Report(const std::string& message, const std::shared_ptr<Token>& token);

  bool HadErrors() const;

  std::string input_file_;
  std::ostream& os_;
  int errors_;
};

} // namespace mcc

#endif //MCC_SRC_ERRORREPORTER_H_
