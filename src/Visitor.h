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
  virtual T Visit(const std::shared_ptr<TypeCast>& type_cast) = 0;
  virtual T Visit(const std::shared_ptr<Typedef>& typedef_stmt) = 0;
  virtual T Visit(const std::shared_ptr<Switch>& switch_stmt) = 0;
  virtual T Visit(const std::shared_ptr<Enum>& enum_decl) = 0;
  virtual T Visit(const std::shared_ptr<Union>& union_decl) = 0;
  virtual T Visit(const std::shared_ptr<Struct>& struct_decl) = 0;
  virtual T Visit(const std::shared_ptr<Label>& label) = 0;
  virtual T Visit(const std::shared_ptr<GoTo>& go_to) = 0;
  virtual T Visit(const std::shared_ptr<Access>& access) = 0;
  virtual T Visit(const std::shared_ptr<Index>& index) = 0;
  virtual T Visit(const std::shared_ptr<Call>& call) = 0;
  virtual T Visit(const std::shared_ptr<Ternary>& ternary) = 0;
  virtual T Visit(const std::shared_ptr<Postfix>& postfix) = 0;
  virtual T Visit(const std::shared_ptr<Grouping>& grouping) = 0;
  virtual T Visit(const std::shared_ptr<Binary>& binary) = 0;
  virtual T Visit(const std::shared_ptr<Unary>& unary) = 0;
  virtual T Visit(const std::shared_ptr<Literal>& literal) = 0;
  virtual T Visit(const std::shared_ptr<Assign>& assign) = 0;
  virtual T Visit(const std::shared_ptr<FuncDecl>& func_decl) = 0;
  virtual T Visit(const std::shared_ptr<VarDecl>& var_decl) = 0;
  virtual T Visit(const std::shared_ptr<Print>& print) = 0;
  virtual T Visit(const std::shared_ptr<ExpressionStmt>& expr_stmt) = 0;
  virtual T Visit(const std::shared_ptr<Conditional>& cond_stmt) = 0;
  virtual T Visit(const std::shared_ptr<Block>& block_stmt) = 0;
  virtual T Visit(const std::shared_ptr<While>& while_stmt) = 0;
  virtual T Visit(const std::shared_ptr<For>& for_stmt) = 0;
  virtual T Visit(const std::shared_ptr<DeclList>& decl_list) = 0;
  virtual T Visit(const std::shared_ptr<ExprList>& expr_list) = 0;
  virtual T Visit(const std::shared_ptr<ControlFlow>& flow_stmt) = 0;
  virtual T Visit(const std::shared_ptr<Return>& return_stmt) = 0;
};

} // namespace mcc

#endif //MCC_SRC_VISITOR_H_
