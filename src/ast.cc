//
// Created by vladyslav on 15.01.20.
//

#include "ast.h"
#include "Visitor.h"

namespace mcc {

int Binary::Accept(Visitor& visitor) {
  return visitor.Visit(shared_from_base<Binary>());
}

int Literal::Accept(Visitor& visitor) {
  return visitor.Visit(shared_from_base<Literal>());
}

bool Expr::IsVariable() {
  return false;
}

bool Literal::IsVariable() {
  //std::cout << "Var name: " << literal_->GetStringValue() << std::endl;
  //std::cout << "IsVariable: " << (!literal_->GetStringValue().empty()) << std::endl;
  return !literal_->GetStringValue().empty();
}

int Print::Accept(Visitor& visitor) {
  return visitor.Visit(shared_from_base<Print>());
}

int Assign::Accept(Visitor& visitor) {
  return visitor.Visit(shared_from_base<Assign>());
}

int VarDecl::Accept(Visitor& visitor) {
  return visitor.Visit(shared_from_base<VarDecl>());
}

bool VarDecl::IsDeclaration() const {
  return true;
}

int ExpressionStmt::Accept(Visitor& visitor) {
  return visitor.Visit(shared_from_base<ExpressionStmt>());
}

int Conditional::Accept(Visitor& visitor) {
  return visitor.Visit(shared_from_base<Conditional>());
}

int Block::Accept(Visitor& visitor) {
  return visitor.Visit(shared_from_base<Block>());
}

int While::Accept(Visitor& visitor) {
  return visitor.Visit(shared_from_base<While>());
}

int For::Accept(Visitor& visitor) {
  return visitor.Visit(shared_from_base<For>());
}

int DeclList::Accept(Visitor& visitor) {
  return visitor.Visit(shared_from_base<DeclList>());
}

bool DeclList::IsDeclaration() const {
  return true;
}

int ExprList::Accept(Visitor& visitor) {
  return visitor.Visit(shared_from_base<ExprList>());
}

int ControlFlow::Accept(Visitor& visitor) {
  return visitor.Visit(shared_from_base<ControlFlow>());
}

} // namespace mcc