//
// Created by vladyslav on 15.01.20.
//

#include "ast.h"
#include "Visitor.h"

namespace mcc {

Type Binary::Accept(Visitor<Type>& visitor) {
  return visitor.Visit(shared_from_base<Binary>());
}

int Binary::Accept(Visitor<int>& visitor) {
  return visitor.Visit(shared_from_base<Binary>());
}

Type Literal::Accept(Visitor<Type>& visitor) {
  return visitor.Visit(shared_from_base<Literal>());
}

int Literal::Accept(Visitor<int>& visitor) {
  return visitor.Visit(shared_from_base<Literal>());
}

bool Expr::IsVariable() {
  return false;
}

bool Expr::IsLvalue() {
  return is_lvalue;
}

bool Literal::IsVariable() {
  return !literal_->GetStringValue().empty();
}

Type Print::Accept(Visitor<Type>& visitor) {
  return visitor.Visit(shared_from_base<Print>());
}

int Print::Accept(Visitor<int>& visitor) {
  return visitor.Visit(shared_from_base<Print>());
}

Type Assign::Accept(Visitor<Type>& visitor) {
  return visitor.Visit(shared_from_base<Assign>());
}

int Assign::Accept(Visitor<int>& visitor) {
  return visitor.Visit(shared_from_base<Assign>());
}

Type VarDecl::Accept(Visitor<Type>& visitor) {
  return visitor.Visit(shared_from_base<VarDecl>());
}

int VarDecl::Accept(Visitor<int>& visitor) {
  return visitor.Visit(shared_from_base<VarDecl>());
}

bool VarDecl::IsDeclaration() const {
  return true;
}

Type ExpressionStmt::Accept(Visitor<Type>& visitor) {
  return visitor.Visit(shared_from_base<ExpressionStmt>());
}

int ExpressionStmt::Accept(Visitor<int>& visitor) {
  return visitor.Visit(shared_from_base<ExpressionStmt>());
}

Type Conditional::Accept(Visitor<Type>& visitor) {
  return visitor.Visit(shared_from_base<Conditional>());
}

int Conditional::Accept(Visitor<int>& visitor) {
  return visitor.Visit(shared_from_base<Conditional>());
}

Type Block::Accept(Visitor<Type>& visitor) {
  return visitor.Visit(shared_from_base<Block>());
}

int Block::Accept(Visitor<int>& visitor) {
  return visitor.Visit(shared_from_base<Block>());
}

Type While::Accept(Visitor<Type>& visitor) {
  return visitor.Visit(shared_from_base<While>());
}

int While::Accept(Visitor<int>& visitor) {
  return visitor.Visit(shared_from_base<While>());
}

Type For::Accept(Visitor<Type>& visitor) {
  return visitor.Visit(shared_from_base<For>());
}

int For::Accept(Visitor<int>& visitor) {
  return visitor.Visit(shared_from_base<For>());
}

Type DeclList::Accept(Visitor<Type>& visitor) {
  return visitor.Visit(shared_from_base<DeclList>());
}

int DeclList::Accept(Visitor<int>& visitor) {
  return visitor.Visit(shared_from_base<DeclList>());
}

bool DeclList::IsDeclaration() const {
  return true;
}

Type ExprList::Accept(Visitor<Type>& visitor) {
  return visitor.Visit(shared_from_base<ExprList>());
}

int ExprList::Accept(Visitor<int>& visitor) {
  return visitor.Visit(shared_from_base<ExprList>());
}

Type ControlFlow::Accept(Visitor<Type>& visitor) {
  return visitor.Visit(shared_from_base<ControlFlow>());
}

int ControlFlow::Accept(Visitor<int>& visitor) {
  return visitor.Visit(shared_from_base<ControlFlow>());
}

} // namespace mcc