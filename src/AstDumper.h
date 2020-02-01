//
// Created by vladyslav on 01.02.20.
//

#ifndef MCC_SRC_ASTDUMPER_H_
#define MCC_SRC_ASTDUMPER_H_

#include "common.h"
#include "Visitor.h"

namespace mcc {

class AstDumper : public Visitor {
 public:
  explicit AstDumper(const std::string& file_name)
    : out_(std::ofstream(file_name, std::ios::out)), spaces_(-TAB_SIZE)
  { }

  void Dump(const std::vector<std::shared_ptr<Stmt>>& stmts);

  int Visit(const std::shared_ptr<Binary>& binary) override;
  int Visit(const std::shared_ptr<Literal>& literal) override;
  int Visit(const std::shared_ptr<Assign>& assign) override;
  int Visit(const std::shared_ptr<VarDecl>& var_decl) override;
  int Visit(const std::shared_ptr<Print>& print) override;
  int Visit(const std::shared_ptr<ExpressionStmt>& expr_stmt) override;
  int Visit(const std::shared_ptr<Conditional>& cond_stmt) override;
  int Visit(const std::shared_ptr<Block>& block_stmt) override;

  void Flush();

  std::ofstream out_;
  int spaces_;
};

} // namespace mcc

#endif //MCC_SRC_ASTDUMPER_H_