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

class TypeChecker : public Visitor<void> {
 public:
  TypeChecker(CodeGenX86& code_gen, ErrorReporter& reporter, SymbolTable& symbol_table)
    : code_gen_(code_gen), reporter_(reporter), symbol_table_(symbol_table), curr_func_(nullptr)
  { }

  void TypeCheck(const std::vector<std::shared_ptr<Stmt>>& stmts);

  void Visit(const std::shared_ptr<Unary>& unary) override;
  void Visit(const std::shared_ptr<Binary>& binary) override;
  void Visit(const std::shared_ptr<Literal>& literal) override;
  void Visit(const std::shared_ptr<Assign>& assign) override;
  void Visit(const std::shared_ptr<VarDecl>& decl) override;
  void Visit(const std::shared_ptr<Print>& print) override;
  void Visit(const std::shared_ptr<ExpressionStmt>& expr_stmt) override;
  void Visit(const std::shared_ptr<Conditional>& cond_stmt) override;
  void Visit(const std::shared_ptr<Block>& block_stmt) override;
  void Visit(const std::shared_ptr<While>& while_stmt) override;
  void Visit(const std::shared_ptr<For>& for_stmt) override;
  void Visit(const std::shared_ptr<DeclList>& decl_list) override;
  void Visit(const std::shared_ptr<ExprList>& expr_list) override;
  void Visit(const std::shared_ptr<ControlFlow>& flow_stmt) override;
  void Visit(const std::shared_ptr<Call>& call) override;
  void Visit(const std::shared_ptr<Grouping>& grouping) override;
  void Visit(const std::shared_ptr<Ternary>& ternary) override;
  void Visit(const std::shared_ptr<Postfix>& postfix) override;
  void Visit(const std::shared_ptr<Index> &index) override;
  void Visit(const std::shared_ptr<Label> &label) override;
  void Visit(const std::shared_ptr<GoTo> &go_to) override;
  void Visit(const std::shared_ptr<FuncDecl>& func_decl) override;
  void Visit(const std::shared_ptr<Return>& return_stmt) override;
  void Visit(const std::shared_ptr<Struct> &decl) override;
  void Visit(const std::shared_ptr<Access> &access) override;

  void NewLabelScope(const std::shared_ptr<FuncDecl> &func_decl);
  void CheckLabelScope(const std::shared_ptr<FuncDecl> &func_decl);

  static bool IsIntegerType(ExprRef expr);
  static bool IsPointer(ExprRef expr);
  static bool IsComparison(TokenType type);

  Type& PromotePrim(ExprRef e1, ExprRef e2);
  Type& PromoteToLeft(ExprRef e1, ExprRef e2);

  bool MatchTypes(ExprRef e1, ExprRef e2, bool to_left, ExprRef expr);

  bool MatchPointers(const std::shared_ptr<Expr>& e1,
                          const std::shared_ptr<Expr>& e2,
                          bool to_left,
                          const std::shared_ptr<Expr>& binary);
  bool MatchPrimitives(const std::shared_ptr<Expr>& e1,
                            const std::shared_ptr<Expr>& e2,
                            bool to_left,
                            const std::shared_ptr<Expr> &binary);
  bool MatchMixed(const std::shared_ptr<Expr>& e1,
                       const std::shared_ptr<Expr>& e2,
                       bool to_left,
                       const std::shared_ptr<Expr>& binary);

  bool MatchTypeInit(const Type& type, const std::shared_ptr<Expr>& init);

  int GetLocalOffset(const Type& type, int len);
  void ResetLocals();
  int GetTypeSize(const Type& type);

  static void FreeEntries(Entry *entry);
  int GetOffset(const Type &type, int len, int offset);

  int unnamed = 0;
  int local_offset_ = 0;
  bool gen_params_ = false; // hack to implement param generation

  CodeGenX86& code_gen_; // #FIXME: rewrite for higher abstraction using `CodeGen : Visitor<int>` as interface
  ErrorReporter& reporter_;
  SymbolTable& symbol_table_;
  std::shared_ptr<FuncDecl> curr_func_;
};

} // namespace mcc

#endif //MCC_SRC_TYPECHECKER_H_
