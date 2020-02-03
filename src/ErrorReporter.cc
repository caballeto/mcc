//
// Created by vladyslav on 03.02.20.
//

#include "ErrorReporter.h"

namespace mcc {

void ErrorReporter::Report(const std::string& message, char c, int line) {
  os_ << message << "'" << c << "', line " << line << std::endl;
  errors_++;
}

void ErrorReporter::Report(const std::string& message, const std::shared_ptr<Token>& token) {
  os_ << message << " token " << token->GetType() << ", line "
      << token->GetLine() << std::endl;
  errors_++;
}

bool ErrorReporter::HadErrors() const {
  return errors_ != 0;
}

} // namespace mcc
