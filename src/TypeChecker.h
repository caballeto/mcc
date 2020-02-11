//
// Created by vladyslav on 03.02.20.
//

#ifndef MCC_SRC_TYPECHECKER_H_
#define MCC_SRC_TYPECHECKER_H_

#include "Visitor.h"
#include "ErrorReporter.h"
#include "SymbolTable.h"
#include "Type.h"
#include "CodeGenX86.h"

namespace mcc {

using ExprRef = const std::shared_ptr<Expr>&;
using TokenRef = const std::shared_ptr<Token>&;

class TypeChecker : public Visitor<Type> {
 public:
  TypeChecker(CodeGenX86& code_gen, ErrorReporter& reporter, SymbolTable& symbol_table)
    : code_gen_(code_gen), reporter_(reporter), symbol_table_(symbol_table), curr_func(nullptr)
  { }

  void TypeCheck(const std::vector<std::shared_ptr<Stmt>>& stmts);

  Type Visit(const std::shared_ptr<Unary>& unary) override;
  Type Visit(const std::shared_ptr<Binary>& binary) override;
  Type Visit(const std::shared_ptr<Literal>& literal) override;
  Type Visit(const std::shared_ptr<Assign>& assign) override;
  Type Visit(const std::shared_ptr<VarDecl>& decl) override;
  Type Visit(const std::shared_ptr<Print>& print) override;
  Type Visit(const std::shared_ptr<ExpressionStmt>& expr_stmt) override;
  Type Visit(const std::shared_ptr<Conditional>& cond_stmt) override;
  Type Visit(const std::shared_ptr<Block>& block_stmt) override;
  Type Visit(const std::shared_ptr<While>& while_stmt) override;
  Type Visit(const std::shared_ptr<For>& for_stmt) override;
  Type Visit(const std::shared_ptr<DeclList>& decl_list) override;
  Type Visit(const std::shared_ptr<ExprList>& expr_list) override;
  Type Visit(const std::shared_ptr<ControlFlow>& flow_stmt) override;
  Type Visit(const std::shared_ptr<Call>& call) override;
  Type Visit(const std::shared_ptr<Grouping>& grouping) override;
  Type Visit(const std::shared_ptr<Ternary>& ternary) override;
  Type Visit(const std::shared_ptr<Postfix>& postfix) override;
  Type Visit(const std::shared_ptr<Index> &index) override;

  static bool IsIntegerType(ExprRef expr);
  static bool IsPointer(ExprRef expr);
  static bool IsComparison(TokenType type);

  Type Promote(ExprRef e1, ExprRef e2);
  Type PromoteToLeft(ExprRef e1, ExprRef e2);
  Type MatchTypes(ExprRef e1, ExprRef e2, bool to_left, ExprRef binary);

  Type MatchPointers(const std::shared_ptr<Expr>& e1,
                     const std::shared_ptr<Expr>& e2,
                     bool to_left,
                     const std::shared_ptr<Expr>& binary);
  Type MatchPrimitives(const std::shared_ptr<Expr>& e1,
                       const std::shared_ptr<Expr>& e2,
                       bool to_left,
                       const std::shared_ptr<Expr> &binary);
  Type MatchMixed(const std::shared_ptr<Expr>& e1,
                  const std::shared_ptr<Expr>& e2,
                  bool to_left,
                  const std::shared_ptr<Expr>& binary);

  bool MatchTypeInit(Type type, int indirection, const std::shared_ptr<Expr>& init);
  Type Visit(const std::shared_ptr<FuncDecl>& func_decl) override;
  Type Visit(const std::shared_ptr<Return>& return_stmt) override;

  int GetLocalOffset(Type type, int ind);
  void ResetLocals();
  static int GetTypeSize(Type type);

  int local_offset_ = 0;

  CodeGenX86& code_gen_; // #FIXME: rewrite for higher abstraction using `CodeGen : Visitor<int>` as interface
  ErrorReporter& reporter_;
  SymbolTable& symbol_table_;
  std::shared_ptr<FuncDecl> curr_func;
};

} // namespace mcc

#endif //MCC_SRC_TYPECHECKER_H_
