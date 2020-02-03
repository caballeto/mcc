//
// Created by vladyslav on 03.02.20.
//

#ifndef MCC_SRC_ERRORREPORTER_H_
#define MCC_SRC_ERRORREPORTER_H_

#include "common.h"
#include "Token.h"

namespace mcc {

class ErrorReporter {
 public:
  explicit ErrorReporter(std::ostream& os)
    : os_(os), errors_(0)
  { }

  void Report(const std::string& message, char c, int line);
  void Report(const std::string& message, const std::shared_ptr<Token>& token);
  bool HadErrors() const;

  std::ostream& os_;
  int errors_;
};

} // namespace mcc

#endif //MCC_SRC_ERRORREPORTER_H_
