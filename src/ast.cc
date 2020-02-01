//
// Created by vladyslav on 15.01.20.
//

#include "ast.h"
#include "Visitor.h"

namespace mcc {

int Binary::Accept(Visitor& visitor) {
  return visitor.Visit(std::static_pointer_cast<Binary>(shared_from_this()));
}

int Literal::Accept(Visitor& visitor) {
  return visitor.Visit(std::static_pointer_cast<Literal>(shared_from_this()));
}

bool Expr::IsVariable() {
  return false;
}

bool Literal::IsVariable() {
  //std::cout << "Var name: " << literal_->GetStringValue() << std::endl;
  //std::cout << "IsVariable: " << (!literal_->GetStringValue().empty()) << std::endl;
  return !literal_->GetStringValue().empty();
}

int Print::Accept(Visitor &visitor) {
  return visitor.Visit(std::static_pointer_cast<Print>(shared_from_this()));
}

int Assign::Accept(Visitor &visitor) {
  return visitor.Visit(std::static_pointer_cast<Assign>(shared_from_this()));
}

int VarDecl::Accept(Visitor &visitor) {
  return visitor.Visit(std::static_pointer_cast<VarDecl>(shared_from_this()));
}

int ExpressionStmt::Accept(Visitor &visitor) {
  return visitor.Visit(std::static_pointer_cast<ExpressionStmt>(shared_from_this()));
}

int Conditional::Accept(Visitor& visitor) {
  return visitor.Visit(std::static_pointer_cast<Conditional>(shared_from_this()));
}

int Block::Accept(Visitor &visitor) {
  return visitor.Visit(std::static_pointer_cast<Block>(shared_from_this()));
}

} // namespace mcc