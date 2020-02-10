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
  if (token->GetType() == TokenType::T_INT_LIT) {
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

void ErrorReporter::Error(
    const std::string& message,
    Type type,
    int indirection,
    const std::shared_ptr<Expr>& expr,
    const std::shared_ptr<Token>& token) {
  PrintErrorLine(input_file_, token->GetLine(), token->GetCount());
  os_ << message << "(";
  PrintType(os_, type, indirection);
  os_ << " and " << expr << ")" << ", near ";

  if (token->GetType() == TokenType::T_INT_LIT) {
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

  if (token->GetType() == TokenType::T_INT_LIT) {
    os_ << "'" << token->GetIntValue() << "'";
  } else if (token->GetType() == TokenType::T_IDENTIFIER) {
    os_ << "'" << token->GetStringValue() << "'";
  } else {
    os_ << token->GetType();
  }

  os_ << ", line " << token->GetLine() << std::endl;
  os_ << std::endl;
}

void ErrorReporter::Warning(const std::string& message,
                            Type type,
                            int indirection,
                            const std::shared_ptr<Expr>& expr,
                            const std::shared_ptr<Token>& token) {
  PrintWarningLine(input_file_, token->GetLine(), token->GetCount());
  os_ << message << "(";
  PrintType(os_, type, indirection);
  os_ << " and " << expr << ")" << ", near ";

  if (token->GetType() == TokenType::T_INT_LIT) {
    os_ << "'" << token->GetIntValue() << "'";
  } else if (token->GetType() == TokenType::T_IDENTIFIER) {
    os_ << "'" << token->GetStringValue() << "'";
  } else {
    os_ << token->GetType();
  }

  os_ << ", line " << token->GetLine() << std::endl;
  os_ << std::endl;
}

} // namespace mcc
