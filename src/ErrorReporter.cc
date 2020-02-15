//
// Created by vladyslav on 03.02.20.
//

#include "ErrorReporter.h"
#include "TokenType.h"
#include "Type.h"
#include "ast.h"

namespace mcc {

void ErrorReporter::Report(const std::string& message, char c, int line, int count) {
  PrintErrorLine(input_file_, line, count);
  os_ << message << ", near '" << c << "', line " << line << std::endl;
  os_ << std::endl;
  errors_++;
}

void ErrorReporter::Report(const std::string& message, const std::shared_ptr<Token>& token) {
  PrintErrorLine(input_file_, token->GetLine(), token->GetCount());
  os_ << message << ", near ";
  PrintToken(token);
  errors_++;
}

void ErrorReporter::Warning(const std::string &message, const std::shared_ptr<Token> &token) {
  PrintWarningLine(input_file_, token->GetLine(), token->GetCount());
  os_ << message << ", near ";
  PrintToken(token);
}

void ErrorReporter::PrintToken(const std::shared_ptr<Token>& token) {
  if (token->GetType() == TokenType::T_INT_LIT) {
    os_ << "'" << token->Int() << "'";
  } else if (token->GetType() == TokenType::T_IDENTIFIER) {
    os_ << "'" << token->String() << "'";
  } else {
    os_ << "'" << token->GetType() << "'";
  }

  os_ << ", line " << token->GetLine() << std::endl;
  os_ << std::endl;
}

void ErrorReporter::Report(const std::string& message) {
  os_ << message << std::endl;
  os_ << std::endl;
  errors_++;
}

std::ostream& operator<<(std::ostream& os, const std::shared_ptr<Expr>& expr) {
  return os << expr->type_;
}

void ErrorReporter::Error(
    const std::string& message,
    const Type& type,
    const std::shared_ptr<Expr>& expr,
    const std::shared_ptr<Token>& token) {
  PrintErrorLine(input_file_, token->GetLine(), token->GetCount());
  os_ << message << "(" << type;
  os_ << " and " << expr << ")" << ", near ";
  PrintToken(token);
  errors_++;
}

void ErrorReporter::Error(
    const std::string& message,
    const std::shared_ptr<Expr>& e1,
    const std::shared_ptr<Expr>& e2,
    const std::shared_ptr<Token>& token) {
  PrintErrorLine(input_file_, token->GetLine(), token->GetCount());
  PrintMessage(message, e1, e2, token);
  errors_++;
}

void ErrorReporter::Warning(const std::string &message,
                            const std::shared_ptr<Expr>& e1,
                            const std::shared_ptr<Expr>& e2,
                            const std::shared_ptr<Token>& token) {
  PrintWarningLine(input_file_, token->GetLine(), token->GetCount());
  PrintMessage(message, e1, e2, token);
}

bool ErrorReporter::HadErrors() const {
  return errors_ != 0;
}

void ErrorReporter::ReportParseError(const ParseException& exception) {
  Report(exception.GetMessage(), exception.GetToken());
}

void ErrorReporter::PrintColoredLine(const std::string& file,
                                     int line,
                                     int c,
                                     const std::string& word,
                                     const std::string& color) {
  os_ << file << ":" << line << ":" << c << color << " " << word << " " << RESET << std::endl;
}

void ErrorReporter::PrintWarningLine(const std::string& file, int line, int c) {
  PrintColoredLine(file, line, c, "warning", CYAN);
}

void ErrorReporter::PrintErrorLine(const std::string& file, int line, int c) {
  PrintColoredLine(file, line, c, "error", RED);
}

void ErrorReporter::PrintMessage(const std::string& message,
                                 const std::shared_ptr<Expr>& e1,
                                 const std::shared_ptr<Expr>& e2,
                                 const std::shared_ptr<Token>& token) {
  os_ << message << "(" << e1 << " and " << e2 << ")" << ", near ";
  PrintToken(token);
}

void ErrorReporter::Warning(const std::string& message,
                            const Type& type,
                            const std::shared_ptr<Expr>& expr,
                            const std::shared_ptr<Token>& token) {
  PrintWarningLine(input_file_, token->GetLine(), token->GetCount());
  os_ << message << "(" << type;
  os_ << " and " << expr << ")" << ", near ";
  PrintToken(token);
}

} // namespace mcc
