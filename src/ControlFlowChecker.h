//
// Created by vladyslav on 03.02.20.
//

#ifndef MCC_SRC_CONTROLFLOWCHECKER_H_
#define MCC_SRC_CONTROLFLOWCHECKER_H_

#include "Visitor.h"

namespace mcc {

class ControlFlowChecker: public Visitor<int> {
 public:
  int Visit(const std::shared_ptr<Binary> &binary) override;
  int Visit(const std::shared_ptr<Literal> &literal) override;
  int Visit(const std::shared_ptr<Assign> &assign) override;
  int Visit(const std::shared_ptr<VarDecl> &var_decl) override;
  int Visit(const std::shared_ptr<Print> &print) override;
  int Visit(const std::shared_ptr<ExpressionStmt> &expr_stmt) override;
  int Visit(const std::shared_ptr<Conditional> &cond_stmt) override;
  int Visit(const std::shared_ptr<Block> &block_stmt) override;
  int Visit(const std::shared_ptr<While> &while_stmt) override;
  int Visit(const std::shared_ptr<For> &for_stmt) override;
  int Visit(const std::shared_ptr<DeclList> &decl_list) override;
  int Visit(const std::shared_ptr<ExprList> &expr_list) override;
  int Visit(const std::shared_ptr<ControlFlow> &flow_stmt) override;
  int Visit(const std::shared_ptr<Unary> &unary) override;
  int Visit(const std::shared_ptr<FuncDecl> &func_decl) override;
  int Visit(const std::shared_ptr<Return> &return_stmt) override;
  int Visit(const std::shared_ptr<Call> &call) override;
  int Visit(const std::shared_ptr<Grouping> &grouping) override;
  int Visit(const std::shared_ptr<Ternary> &ternary) override;
  int Visit(const std::shared_ptr<Postfix> &postfix) override;
  int Visit(const std::shared_ptr<Index> &index) override;
  int Visit(const std::shared_ptr<Label> &label) override;
  int Visit(const std::shared_ptr<GoTo> &go_to) override;
  int Visit(const std::shared_ptr<Struct> &struct_decl) override;
  int Visit(const std::shared_ptr<Access> &access) override;
  int Visit(const std::shared_ptr<Union> &union_decl) override;
  int Visit(const std::shared_ptr<Enum> &enum_decl) override;
  int Visit(const std::shared_ptr<Switch> &switch_stmt) override;
  int Visit(const std::shared_ptr<Typedef> &typedef_stmt) override;
  int Visit(const std::shared_ptr<TypeCast> &type_cast) override;
};

} // namespace mcc

#endif //MCC_SRC_CONTROLFLOWCHECKER_H_
