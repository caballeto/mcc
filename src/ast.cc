//
// Created by vladyslav on 15.01.20.
//

#include "ast.h"
#include "Visitor.h"

namespace mcc {

// #TODO: Rewrite Dump as Visitor - AST Printer

void Stmt::Dump(std::ostream &os, int spaces, const std::vector<std::shared_ptr<Stmt>>& stmts) {
  for (const auto& stmt : stmts) {
    stmt->Dump(os, spaces);
  }
}

void Binary::Dump(std::ostream& os, int spaces) {
  os << std::string(spaces, ' ') << "<binary op='" << op_->GetType() << "'>" << "\n";
  left_->Dump(os, spaces + 2);
  right_->Dump(os, spaces + 2);
  os << std::string(spaces, ' ') << "</binary>" << "\n";
}

int Binary::Accept(Visitor& visitor) {
  return visitor.Visit(std::static_pointer_cast<Binary>(shared_from_this()));
}

void Literal::Dump(std::ostream& os, int spaces) {
  os << std::string(spaces, ' ')
    << "<literal val='";
  if (!literal_->GetStringValue().empty())
    os << literal_->GetStringValue();
  else
    os << literal_->GetIntValue();
  os << "'></literal>" << "\n";
}

int Literal::Accept(Visitor& visitor) {
  return visitor.Visit(std::static_pointer_cast<Literal>(shared_from_this()));
}

bool Expr::IsVariable() {
  return false;
}

bool Literal::IsVariable() {
  std::cout << "Var name: " << literal_->GetStringValue() << std::endl;
  std::cout << "IsVariable: " << (!literal_->GetStringValue().empty()) << std::endl;
  return !literal_->GetStringValue().empty();
}

void Print::Dump(std::ostream &os, int spaces) {
  os << std::string(spaces, ' ') << "<stmt type='print'>\n";
  expr_->Dump(os, spaces + 2);
  os << std::string(spaces, ' ') << "</stmt>\n";
}

int Print::Accept(Visitor &visitor) {
  return visitor.Visit(std::static_pointer_cast<Print>(shared_from_this()));
}

int Assign::Accept(Visitor &visitor) {
  return visitor.Visit(std::static_pointer_cast<Assign>(shared_from_this()));
}

void Assign::Dump(std::ostream &os, int spaces) {
  os << std::string(spaces, ' ') << "<assign>\n";
  left_->Dump(os, spaces + 2);
  right_->Dump(os, spaces + 2);
  os << std::string(spaces, ' ') << "</assign>\n";
}

int VarDecl::Accept(Visitor &visitor) {
  return visitor.Visit(std::static_pointer_cast<VarDecl>(shared_from_this()));
}

void VarDecl::Dump(std::ostream &os, int spaces) {
  os << std::string(spaces, ' ') << "<var-decl name='" << name_ << "' />\n";
}

void ExpressionStmt::Dump(std::ostream &os, int spaces) {
  os << std::string(spaces, ' ') << "<expr-stmt>\n";
  expr_->Dump(os, spaces + 2);
  os << std::string(spaces, ' ') << "</expr-stmt>" << std::endl;
}

int ExpressionStmt::Accept(Visitor &visitor) {
  return visitor.Visit(std::static_pointer_cast<ExpressionStmt>(shared_from_this()));
}

void Conditional::Dump(std::ostream& os, int spaces) {
  os << std::string(spaces, ' ') << "<if-stmt>\n";
  os << std::string(spaces + 2, ' ') << "<condition>\n";
  condition_->Dump(os, spaces + 4);
  os << std::string(spaces + 2, ' ') << "</condition>\n";
  os << std::string(spaces + 2, ' ') << "<if>\n";
  Stmt::Dump(os, spaces + 4, then_block_->stmts_);
  os << std::string(spaces + 2, ' ') << "</if>\n";

  if (!then_block_->stmts_.empty()) {
    os << std::string(spaces + 2, ' ') << "<else>\n";
    Stmt::Dump(os, spaces + 4, else_block_->stmts_);
    os << std::string(spaces + 2, ' ') << "</else>\n";
  }

  os << std::string(spaces, ' ') << "</if-stmt>" << std::endl;
}

int Conditional::Accept(Visitor& visitor) {
  return visitor.Visit(std::static_pointer_cast<Conditional>(shared_from_this()));
}

int Block::Accept(Visitor &visitor) {
  return visitor.Visit(std::static_pointer_cast<Block>(shared_from_this()));
}

void Block::Dump(std::ostream &os, int spaces) {
  os << std::string(spaces, ' ') << "</if-stmt>" << std::endl;
  Stmt::Dump(os, spaces, stmts_);
  os << std::string(spaces, ' ') << "</if-stmt>" << std::endl;
}

} // namespace mcc