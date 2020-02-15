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
    : scanner_(scanner), reporter_(reporter), token_(nullptr), look_ahead_(nullptr)
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
  std::shared_ptr<Token> PeekNext();
  std::shared_ptr<Token> Consume(TokenType type, const std::string& message);
  std::shared_ptr<Token> Consume(TokenType type);

  bool Check(TokenType type);
  bool Match(TokenType type);

  std::shared_ptr<While> WhileStatement();
  std::shared_ptr<For> ForStatement();
  std::shared_ptr<While> DoWhileStatement();
  std::shared_ptr<Conditional> IfStatement();
  std::shared_ptr<Print> PrintStatement();
  std::shared_ptr<ControlFlow> BreakStatement();
  std::shared_ptr<ControlFlow> ContinueStatement();
  std::shared_ptr<GoTo> GotoStatement();


  bool MatchType();
  static void DisallowDecl(const std::shared_ptr<Stmt>& stmt, const std::shared_ptr<Token>& token);
  void Synchronize();
  void SynchronizeDeclaration();
  static bool IsStopToken(TokenType type);

  std::shared_ptr<Stmt> Declaration();
  std::shared_ptr<Stmt> GlobalVarDeclaration(Type type, std::shared_ptr<Token> name);
  std::shared_ptr<Struct> StructDeclaration(const Type& type);
  std::shared_ptr<Return> ReturnStatement();

  std::shared_ptr<DeclList> ParameterList(TokenType delim, TokenType stop);
  Scanner& scanner_;
  ErrorReporter& reporter_;
  std::shared_ptr<Token> token_;
  std::shared_ptr<Token> look_ahead_;
  Type ParsePrim();
};

} // namespace mcc

#endif //MCC_SRC_PARSER_H_
