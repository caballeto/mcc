//
// Created by vladyslav on 15.01.20.
//

#ifndef MCC_SRC_PARSER_H_
#define MCC_SRC_PARSER_H_

#include "Scanner.h"
#include "ast.h"

namespace mcc {

class Parser {
 public:
  explicit Parser(Scanner& scanner)
    : scanner_(scanner)
  { Next(); }

  std::shared_ptr<Expr> Expression(int precedence);
  std::shared_ptr<Expr> Primary();

 private:
  std::shared_ptr<Token> Next();
  std::shared_ptr<Token> Peek();
  bool Match(TokenType type);

  std::shared_ptr<Token> token_;
  Scanner& scanner_;
};

} // namespace mcc

#endif //MCC_SRC_PARSER_H_
