//
// Created by vladyslav on 16.01.20.
//

#ifndef MCC_SRC_VISITOR_H_
#define MCC_SRC_VISITOR_H_

#include "ast.h"

namespace mcc {

template <typename T>
class Visitor {
 public:
  virtual T Visit(TypeCast& type_cast) = 0;
  virtual T Visit(Typedef& typedef_stmt) = 0;
  virtual T Visit(Switch& switch_stmt) = 0;
  virtual T Visit(Enum& enum_decl) = 0;
  virtual T Visit(Union& union_decl) = 0;
  virtual T Visit(Struct& struct_decl) = 0;
  virtual T Visit(Label& label) = 0;
  virtual T Visit(GoTo& go_to) = 0;
  virtual T Visit(Access& access) = 0;
  virtual T Visit(Index& index) = 0;
  virtual T Visit(Call& call) = 0;
  virtual T Visit(Ternary& ternary) = 0;
  virtual T Visit(Postfix& postfix) = 0;
  virtual T Visit(Grouping& grouping) = 0;
  virtual T Visit(Binary& binary) = 0;
  virtual T Visit(Unary& unary) = 0;
  virtual T Visit(Literal& literal) = 0;
  virtual T Visit(Assign& assign) = 0;
  virtual T Visit(FuncDecl& func_decl) = 0;
  virtual T Visit(VarDecl& var_decl) = 0;
  virtual T Visit(Print& print) = 0;
  virtual T Visit(ExpressionStmt& expr_stmt) = 0;
  virtual T Visit(Conditional& cond_stmt) = 0;
  virtual T Visit(Block& block_stmt) = 0;
  virtual T Visit(While& while_stmt) = 0;
  virtual T Visit(For& for_stmt) = 0;
  virtual T Visit(DeclList& decl_list) = 0;
  virtual T Visit(ExprList& expr_list) = 0;
  virtual T Visit(ControlFlow& flow_stmt) = 0;
  virtual T Visit(Return& return_stmt) = 0;
};

} // namespace mcc

#endif //MCC_SRC_VISITOR_H_
