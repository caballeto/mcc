//
// Created by vladyslav on 16.01.20.
//

#ifndef MCC_SRC_VISITOR_H_
#define MCC_SRC_VISITOR_H_

#include "ast.h"

namespace mcc {

class Visitor {
 public:
  virtual int Visit(const std::shared_ptr<Binary>& binary) = 0;
  virtual int Visit(const std::shared_ptr<Literal>& literal) = 0;
  virtual int Visit(const std::shared_ptr<Assign>& assign) = 0;
  virtual int Visit(const std::shared_ptr<VarDecl>& var_decl) = 0;
  virtual int Visit(const std::shared_ptr<Print>& print) = 0;
  virtual int Visit(const std::shared_ptr<ExpressionStmt>& expr_stmt) = 0;
  virtual int Visit(const std::shared_ptr<Conditional>& cond_stmt) = 0;
  virtual int Visit(const std::shared_ptr<Block>& block_stmt) = 0;
  virtual int Visit(const std::shared_ptr<While>& while_stmt) = 0;
  virtual int Visit(const std::shared_ptr<For>& for_stmt) = 0;
  virtual int Visit(const std::shared_ptr<DeclList>& decl_list) = 0;
  virtual int Visit(const std::shared_ptr<ExprList>& expr_list) = 0;
  virtual int Visit(const std::shared_ptr<ControlFlow>& flow_stmt) = 0;
};

} // namespace mcc

#endif //MCC_SRC_VISITOR_H_
