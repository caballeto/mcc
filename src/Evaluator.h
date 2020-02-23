//
// Created by vladyslav on 23.02.20.
//

#ifndef MCC_SRC_EVALUATOR_H_
#define MCC_SRC_EVALUATOR_H_

#include "Visitor.h"
#include "ErrorReporter.h"

namespace mcc {

class Evaluator : public Visitor<long> {
 public:
  explicit Evaluator(ErrorReporter& reporter)
    : reporter_(reporter)
  { }

  long Visit(Binary& binary) override;
  long Visit(Unary& unary) override;
  long Visit(Literal& literal) override;
  long Visit(Assign& assign) override;
  long Visit(VarDecl& var_decl) override;
  long Visit(Print& print) override;
  long Visit(ExpressionStmt& expr_stmt) override;
  long Visit(Conditional& cond_stmt) override;
  long Visit(Block& block_stmt) override;
  long Visit(While& while_stmt) override;
  long Visit(For& for_stmt) override;
  long Visit(DeclList& decl_list) override;
  long Visit(ExprList& expr_list) override;
  long Visit(ControlFlow& flow_stmt) override;
  long Visit(FuncDecl& func_decl) override;
  long Visit(Return& return_stmt) override;
  long Visit(Call& call) override;
  long Visit(Index &index) override;
  long Visit(Grouping &grouping) override;
  long Visit(Ternary &ternary) override;
  long Visit(Postfix &postfix) override;
  long Visit(Label &label) override;
  long Visit(GoTo &go_to) override;
  long Visit(Struct &struct_decl) override;
  long Visit(Access &access) override;
  long Visit(Union &union_decl) override;
  long Visit(Enum &enum_decl) override;
  long Visit(Switch &switch_stmt) override;
  long Visit(Typedef &typedef_stmt) override;
  long Visit(TypeCast &type_cast) override;

  ErrorReporter& reporter_;
};

} // namespace mcc

#endif //MCC_SRC_EVALUATOR_H_
