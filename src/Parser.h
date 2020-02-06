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
  explicit Parser(Scanner& scanner, ErrorReporter& reporter)
    : scanner_(scanner), reporter_(reporter)
  { Next(); }

  std::vector<std::shared_ptr<Stmt>> Parse();

 private:
  std::shared_ptr<DeclList> DeclarationList();
  std::shared_ptr<ExprList> ExpressionList();

  std::shared_ptr<Expr> OptionalExpression(int precedence);
  std::shared_ptr<Expr> Expression(int precedence);
  std::shared_ptr<Expr> Primary();

  std::shared_ptr<Stmt> Statement();
  std::shared_ptr<Stmt> ExpressionStatement();
  std::shared_ptr<Block> BlockStatement();

  std::shared_ptr<Token> Next();
  std::shared_ptr<Token> Peek();
  std::shared_ptr<Token> Consume(TokenType type, const std::string& message);

  bool Check(TokenType type);
  bool Match(TokenType type);

  std::shared_ptr<While> WhileStatement();
  std::shared_ptr<For> ForStatement();
  std::shared_ptr<While> DoWhileStatement();
  std::shared_ptr<Conditional> IfStatement();
  std::shared_ptr<Print> PrintStatement();

  std::shared_ptr<ControlFlow> BreakStatement();
  std::shared_ptr<ControlFlow> ContinueStatement();
  bool MatchType();

  static void DisallowDecl(const std::shared_ptr<Stmt>& stmt, const std::shared_ptr<Token>& token);
  void synchronize();

  std::shared_ptr<Token> Consume(TokenType type);
  static bool IsStopToken(TokenType type);
  std::shared_ptr<Stmt> Declaration();
  std::shared_ptr<Stmt> GlobalVarDeclaration(const std::shared_ptr<Token>& type_token, int indirection, std::shared_ptr<Token> name);
  std::shared_ptr<Return> ReturnStatement();
  std::shared_ptr<DeclList> ParameterList();

  Scanner& scanner_;
  ErrorReporter& reporter_;
  std::shared_ptr<Token> token_;
};

} // namespace mcc

#endif //MCC_SRC_PARSER_H_
