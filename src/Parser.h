//
// Created by vladyslav on 15.01.20.
//

#ifndef MCC_SRC_PARSER_H_
#define MCC_SRC_PARSER_H_

#include "Scanner.h"
#include "ast.h"
#include "SymbolTable.h"

namespace mcc {

class Parser {
 public:
  explicit Parser(Scanner& scanner, SymbolTable& symbol_table)
    : scanner_(scanner), symbol_table_(symbol_table)
  { Next(); }

  std::vector<std::shared_ptr<Stmt>> Parse();

  std::shared_ptr<Expr> Expression(int precedence);
  std::shared_ptr<Expr> Primary();

 private:
  std::shared_ptr<Token> Next();
  std::shared_ptr<Token> Peek();
  std::shared_ptr<Token> Consume(TokenType type, const std::string& message);
  bool Match(TokenType type);

  std::shared_ptr<Token> token_;
  Scanner& scanner_;
  SymbolTable& symbol_table_;
  std::shared_ptr<Stmt> Statement();
  std::shared_ptr<Stmt> ExpressionStatement();
  std::shared_ptr<Expr> AssignExpression();
};

} // namespace mcc

#endif //MCC_SRC_PARSER_H_
