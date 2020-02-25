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
#include "Evaluator.h"

namespace mcc {

using TokenRef = const std::shared_ptr<Token>&;

class TypeChecker : public Visitor<void> {
 public:
  TypeChecker(CodeGenX86& code_gen, ErrorReporter& reporter, SymbolTable& symbol_table, Evaluator& evaluator)
    : code_gen_(code_gen), reporter_(reporter), symbol_table_(symbol_table), curr_func_(nullptr), evaluator_(evaluator)
  { }

  void TypeCheck(const std::vector<std::shared_ptr<Stmt>>& stmts);

  void Visit(Binary& binary) override;
  void Visit(Unary& unary) override;
  void Visit(Literal& literal) override;
  void Visit(Assign& assign) override;
  void Visit(VarDecl& var_decl) override;
  void Visit(Print& print) override;
  void Visit(ExpressionStmt& expr_stmt) override;
  void Visit(Conditional& cond_stmt) override;
  void Visit(Block& block_stmt) override;
  void Visit(While& while_stmt) override;
  void Visit(For& for_stmt) override;
  void Visit(DeclList& decl_list) override;
  void Visit(ExprList& expr_list) override;
  void Visit(ControlFlow& flow_stmt) override;
  void Visit(FuncDecl& func_decl) override;
  void Visit(Return& return_stmt) override;
  void Visit(Call& call) override;
  void Visit(Index &index) override;
  void Visit(Grouping &grouping) override;
  void Visit(Ternary &ternary) override;
  void Visit(Postfix &postfix) override;
  void Visit(Label &label) override;
  void Visit(GoTo &go_to) override;
  void Visit(Struct &struct_decl) override;
  void Visit(Access &access) override;
  void Visit(Union &union_decl) override;
  void Visit(Enum &enum_decl) override;
  void Visit(Switch &switch_stmt) override;
  void Visit(Typedef &typedef_stmt) override;
  void Visit(TypeCast &type_cast) override;
  void Visit(Logical &logical) override;

  void NewLabelScope(FuncDecl &func_decl);
  void CheckLabelScope(FuncDecl &func_decl);

  static bool IsIntegerType(Expr* expr);
  static bool IsPointer(Expr* expr);
  static bool IsComparison(TokenType type);

  Type& PromotePrim(Expr* e1, Expr* e2);
  Type& PromoteToLeft(Expr* e1, Expr* e2);

  bool MatchTypes(Expr* e1, Expr* e2, bool to_left, Expr* expr);

  bool MatchPointers(Expr* e1, Expr* e2, bool to_left, Expr* binary);
  bool MatchPrimitives(Expr* e1, Expr* e2, bool to_left, Expr* binary);
  bool MatchMixed(Expr* e1, Expr* e2, bool to_left, Expr* binary);
  bool MatchTypeInit(const Type& type, Expr* init);

  int GetLocalOffset(const Type& type, int len);
  void ResetLocals();
  int GetTypeSize(const Type& type);

  static void FreeEntries(Entry *entry);
  int GetOffset(const Type &type, int len);
  void RevertOffsets(Entry *fields, int size);

  int unnamed = 0;
  int local_offset_ = 0;
  bool gen_params_ = false; // hack to implement param generation

  CodeGenX86& code_gen_; // #FIXME: rewrite for higher abstraction using `CodeGen : Visitor<int>` as interface
  ErrorReporter& reporter_;
  SymbolTable& symbol_table_;
  Evaluator& evaluator_;
  FuncDecl* curr_func_;
  void TypedefChange(Type &type);
  int GetSizeOf(const Type &type);
};

} // namespace mcc

#endif //MCC_SRC_TYPECHECKER_H_
