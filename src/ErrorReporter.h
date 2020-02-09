//
// Created by vladyslav on 03.02.20.
//

#ifndef MCC_SRC_ERRORREPORTER_H_
#define MCC_SRC_ERRORREPORTER_H_

#include "common.h"
#include "Type.h"
#include "Token.h"
#include "ParseException.h"
#include "ast.h"

#define CYAN "\033[36m"
#define RED "\033[0;31m"
#define RESET "\033[0m"

namespace mcc {

std::ostream& operator<<(std::ostream& os, const std::shared_ptr<Expr>& expr);

class ErrorReporter {
 public:
  explicit ErrorReporter(std::ostream& os)
    : os_(os), errors_(0)
  { }

  void SetFile(const std::string& input_file) {
    input_file_ = input_file;
  }

  void PrintWarningLine(const std::string& file, int line, int c);
  void PrintErrorLine(const std::string& file, int line, int c);
  void PrintColoredLine(const std::string& file, int line, int c, const std::string& word, const std::string& color);

  void ReportParseError(const ParseException& exception);

  void Error(const std::string& message,
             const std::shared_ptr<Expr>& e1,
             const std::shared_ptr<Expr>& e2,
             const std::shared_ptr<Token>& token);

  void Error(const std::string& message,
             Type type,
             int indirection,
             const std::shared_ptr<Expr>& expr,
             const std::shared_ptr<Token>& token);

  void Warning(const std::string& message,
               const std::shared_ptr<Expr>& e1,
               const std::shared_ptr<Expr>& e2,
               const std::shared_ptr<Token>& token);

  void Report(const std::string& message);
  void Report(const std::string& message, char c, int line, int count);

  void Report(const std::string& message, const std::shared_ptr<Token>& token);

  bool HadErrors() const;

  static void PrintType(std::ostream& os, Type type, int indirection);
  std::string input_file_;
  std::ostream& os_;

  int errors_;
  void PrintMessage(const std::string &message,
                    const std::shared_ptr<Expr> &e1,
                    const std::shared_ptr<Expr> &e2,
                    const std::shared_ptr<Token> &token);
};

} // namespace mcc

#endif //MCC_SRC_ERRORREPORTER_H_
