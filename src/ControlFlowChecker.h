//
// Created by vladyslav on 03.02.20.
//

#ifndef MCC_SRC_CONTROLFLOWCHECKER_H_
#define MCC_SRC_CONTROLFLOWCHECKER_H_

#include "Visitor.h"

namespace mcc {

class ControlFlowChecker: public Visitor<int> {
 public:
  int Visit(Binary& binary) override;
  int Visit(Unary& unary) override;
  int Visit(Literal& literal) override;
  int Visit(Assign& assign) override;
  int Visit(VarDecl& var_decl) override;
  int Visit(Print& print) override;
  int Visit(ExpressionStmt& expr_stmt) override;
  int Visit(Conditional& cond_stmt) override;
  int Visit(Block& block_stmt) override;
  int Visit(While& while_stmt) override;
  int Visit(For& for_stmt) override;
  int Visit(DeclList& decl_list) override;
  int Visit(ExprList& expr_list) override;
  int Visit(ControlFlow& flow_stmt) override;
  int Visit(FuncDecl& func_decl) override;
  int Visit(Return& return_stmt) override;
  int Visit(Call& call) override;
  int Visit(Index &index) override;
  int Visit(Grouping &grouping) override;
  int Visit(Ternary &ternary) override;
  int Visit(Postfix &postfix) override;
  int Visit(Label &label) override;
  int Visit(GoTo &go_to) override;
  int Visit(Struct &struct_decl) override;
  int Visit(Access &access) override;
  int Visit(Union &union_decl) override;
  int Visit(Enum &enum_decl) override;
  int Visit(Switch &switch_stmt) override;
  int Visit(Typedef &typedef_stmt) override;
  int Visit(TypeCast &type_cast) override;
  int Visit(Logical &logical) override;
};

} // namespace mcc

#endif //MCC_SRC_CONTROLFLOWCHECKER_H_
