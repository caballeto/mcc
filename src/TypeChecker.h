//
// Created by vladyslav on 03.02.20.
//

#ifndef MCC_SRC_TYPECHECKER_H_
#define MCC_SRC_TYPECHECKER_H_

#include "Visitor.h"
#include "ErrorReporter.h"

namespace mcc {

class TypeChecker : public Visitor {
 public:
  explicit TypeChecker(ErrorReporter& reporter)
    : reporter_(reporter)
  { }

  void TypeCheck(const std::vector<std::shared_ptr<Stmt>>& stmts);

  int Visit(const std::shared_ptr<Binary>& binary) override;
  int Visit(const std::shared_ptr<Literal>& literal) override;
  int Visit(const std::shared_ptr<Assign>& assign) override;
  int Visit(const std::shared_ptr<VarDecl>& var_decl) override;
  int Visit(const std::shared_ptr<Print>& print) override;
  int Visit(const std::shared_ptr<ExpressionStmt>& expr_stmt) override;
  int Visit(const std::shared_ptr<Conditional>& cond_stmt) override;
  int Visit(const std::shared_ptr<Block>& block_stmt) override;
  int Visit(const std::shared_ptr<While>& while_stmt) override;
  int Visit(const std::shared_ptr<For>& for_stmt) override;
  int Visit(const std::shared_ptr<DeclList>& decl_list) override;
  int Visit(const std::shared_ptr<ExprList>& expr_list) override;
  int Visit(const std::shared_ptr<ControlFlow>& flow_stmt) override;

  ErrorReporter& reporter_;
};

} // namespace mcc

#endif //MCC_SRC_TYPECHECKER_H_
