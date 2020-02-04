//
// Created by vladyslav on 03.02.20.
//

#include "ErrorReporter.h"
#include "TokenType.h"
#include "Type.h"

namespace mcc {

void ErrorReporter::Report(const std::string& message, char c, int line) {
  os_ << message << "'" << c << "', line " << line << std::endl;
  errors_++;
  exit(1);
}

void ErrorReporter::Report(const std::string& message, const std::shared_ptr<Token>& token) {
  os_ << message << " token " << token->GetType() << ", line "
      << token->GetLine() << std::endl;
  errors_++;
  exit(1);
}

void ErrorReporter::Report(const std::string& message) {
  os_ << message << std::endl;
  errors_++;
  exit(1);
}

void ErrorReporter::ReportSemanticError(const std::string& message, Type t1, Type t2) {
  os_ << message << "(" << t1 << " and " << t2 << ")" << std::endl;
  errors_++;
  exit(1);
}

bool ErrorReporter::HadErrors() const {
  return errors_ != 0;
}

} // namespace mcc
