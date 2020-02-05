//
// Created by vladyslav on 03.02.20.
//

#include "ErrorReporter.h"
#include "TokenType.h"
#include "Type.h"
#include "ast.h"

namespace mcc {

void ErrorReporter::Report(const std::string& message, char c, int line) {
  os_ << message << "'" << c << "', line " << line << std::endl;
  errors_++;
}

void ErrorReporter::Report(const std::string& message, const std::shared_ptr<Token>& token) {
  PrintErrorLine(input_file_, token->GetLine(), token->GetCount());

  os_ << message << ", near ";
  if (token->GetType() == TokenType::T_INT_LITERAL) {
    os_ << "'" << token->GetIntValue() << "'";
  } else if (token->GetType() == TokenType::T_IDENTIFIER) {
    os_ << "'" << token->GetStringValue() << "'";
  } else {
    os_ << token->GetType();
  }

  os_ << ", line " << token->GetLine() << std::endl;
  os_ << std::endl;
  errors_++;
}

void ErrorReporter::Report(const std::string& message) {
  os_ << message << std::endl;
  os_ << std::endl;
  errors_++;
}

void ErrorReporter::PrintType(std::ostream& os, Type type, int indirection) {
  os << "'" << type;
  for (int i = 0; i < indirection; i++)
    os << '*';
  os << "'";
}

std::ostream& operator<<(std::ostream& os, const std::shared_ptr<Expr>& expr) {
  ErrorReporter::PrintType(os, expr->type_, expr->indirection_);
  return os;
}

void ErrorReporter::ReportSemanticError(
    const std::string& message,
    Type type,
    int indirection,
    const std::shared_ptr<Expr>& expr,
    const std::shared_ptr<Token>& token) {
  PrintErrorLine(input_file_, token->GetLine(), token->GetCount());
  os_ << message << "(";
  PrintType(os_, type, indirection);
  os_ << " and " << expr << ")" << ", near ";

  if (token->GetType() == TokenType::T_INT_LITERAL) {
    os_ << "'" << token->GetIntValue() << "'";
  } else if (token->GetType() == TokenType::T_IDENTIFIER) {
    os_ << "'" << token->GetStringValue() << "'";
  } else {
    os_ << token->GetType();
  }

  os_ << ", line " << token->GetLine() << std::endl;
  os_ << std::endl;
  errors_++;
}

void ErrorReporter::ReportSemanticError(
    const std::string& message,
    const std::shared_ptr<Expr>& e1,
    const std::shared_ptr<Expr>& e2,
    const std::shared_ptr<Token>& token) {
  PrintErrorLine(input_file_, token->GetLine(), token->GetCount());
  os_ << message << "(" << e1 << " and " << e2 << ")" << ", near ";

  if (token->GetType() == TokenType::T_INT_LITERAL) {
    os_ << "'" << token->GetIntValue() << "'";
  } else if (token->GetType() == TokenType::T_IDENTIFIER) {
    os_ << "'" << token->GetStringValue() << "'";
  } else {
    os_ << token->GetType();
  }

  os_ << ", line " << token->GetLine() << std::endl;
  os_ << std::endl;
  errors_++;
}

bool ErrorReporter::HadErrors() const {
  return errors_ != 0;
}

void ErrorReporter::ReportParseError(const ParseException& exception) {
  Report(exception.GetMessage(), exception.GetToken());
}

void ErrorReporter::PrintErrorLine(const std::string& file, int line, int c) {
  os_ << file << ":" << line << ":" << c << RED << " error " << RESET << std::endl;
}

} // namespace mcc
