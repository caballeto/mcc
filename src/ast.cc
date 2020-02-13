//
// Created by vladyslav on 15.01.20.
//

#include "ast.h"
#include "Visitor.h"

namespace mcc {

Type Binary::Accept(Visitor<Type> &visitor) {
  return visitor.Visit(shared_from_base<Binary>());
}

int Binary::Accept(Visitor<int> &visitor) {
  return visitor.Visit(shared_from_base<Binary>());
}

Type Literal::Accept(Visitor<Type> &visitor) {
  return visitor.Visit(shared_from_base<Literal>());
}

int Literal::Accept(Visitor<int> &visitor) {
  return visitor.Visit(shared_from_base<Literal>());
}

bool Literal::IsArray() {
  return is_array;
}

bool Expr::IsVariable() {
  return false;
}

bool Expr::IsLvalue() {
  return is_lvalue_;
}

bool Literal::IsVariable() {
  return !op_->String().empty();
}

Type Print::Accept(Visitor<Type> &visitor) {
  return visitor.Visit(shared_from_base<Print>());
}

int Print::Accept(Visitor<int> &visitor) {
  return visitor.Visit(shared_from_base<Print>());
}

Type Assign::Accept(Visitor<Type> &visitor) {
  return visitor.Visit(shared_from_base<Assign>());
}

int Assign::Accept(Visitor<int> &visitor) {
  return visitor.Visit(shared_from_base<Assign>());
}

Type VarDecl::Accept(Visitor<Type> &visitor) {
  return visitor.Visit(shared_from_base<VarDecl>());
}

int VarDecl::Accept(Visitor<int> &visitor) {
  return visitor.Visit(shared_from_base<VarDecl>());
}

bool VarDecl::IsDeclaration() const {
  return true;
}

bool VarDecl::IsArray() const {
  return array_len_ > 0;
}

Type ExpressionStmt::Accept(Visitor<Type> &visitor) {
  return visitor.Visit(shared_from_base<ExpressionStmt>());
}

int ExpressionStmt::Accept(Visitor<int> &visitor) {
  return visitor.Visit(shared_from_base<ExpressionStmt>());
}

Type Conditional::Accept(Visitor<Type> &visitor) {
  return visitor.Visit(shared_from_base<Conditional>());
}

int Conditional::Accept(Visitor<int> &visitor) {
  return visitor.Visit(shared_from_base<Conditional>());
}

Type Block::Accept(Visitor<Type> &visitor) {
  return visitor.Visit(shared_from_base<Block>());
}

int Block::Accept(Visitor<int> &visitor) {
  return visitor.Visit(shared_from_base<Block>());
}

Type While::Accept(Visitor<Type> &visitor) {
  return visitor.Visit(shared_from_base<While>());
}

int While::Accept(Visitor<int> &visitor) {
  return visitor.Visit(shared_from_base<While>());
}

Type For::Accept(Visitor<Type> &visitor) {
  return visitor.Visit(shared_from_base<For>());
}

int For::Accept(Visitor<int> &visitor) {
  return visitor.Visit(shared_from_base<For>());
}

Type DeclList::Accept(Visitor<Type> &visitor) {
  return visitor.Visit(shared_from_base<DeclList>());
}

int DeclList::Accept(Visitor<int> &visitor) {
  return visitor.Visit(shared_from_base<DeclList>());
}

bool DeclList::IsDeclaration() const {
  return true;
}

Type ExprList::Accept(Visitor<Type> &visitor) {
  return visitor.Visit(shared_from_base<ExprList>());
}

int ExprList::Accept(Visitor<int> &visitor) {
  return visitor.Visit(shared_from_base<ExprList>());
}

Type ControlFlow::Accept(Visitor<Type> &visitor) {
  return visitor.Visit(shared_from_base<ControlFlow>());
}

int ControlFlow::Accept(Visitor<int> &visitor) {
  return visitor.Visit(shared_from_base<ControlFlow>());
}

int Unary::Accept(Visitor<int> &visitor) {
  return visitor.Visit(shared_from_base<Unary>());
}

Type Unary::Accept(Visitor<Type> &visitor) {
  return visitor.Visit(shared_from_base<Unary>());
}

bool FuncDecl::IsDeclaration() const {
  return true;
}

Type FuncDecl::Accept(Visitor<Type> &visitor) {
  return visitor.Visit(shared_from_base<FuncDecl>());
}

int FuncDecl::Accept(Visitor<int> &visitor) {
  return visitor.Visit(shared_from_base<FuncDecl>());
}

int Return::Accept(Visitor<int> &visitor) {
  return visitor.Visit(shared_from_base<Return>());
}

Type Return::Accept(Visitor<Type> &visitor) {
  return visitor.Visit(shared_from_base<Return>());
}

int Call::Accept(Visitor<int> &visitor) {
  return visitor.Visit(shared_from_base<Call>());
}

Type Call::Accept(Visitor<Type> &visitor) {
  return visitor.Visit(shared_from_base<Call>());
}

int Grouping::Accept(Visitor<int> &visitor) {
  return visitor.Visit(shared_from_base<Grouping>());
}

Type Grouping::Accept(Visitor<Type> &visitor) {
  return visitor.Visit(shared_from_base<Grouping>());
}

int Postfix::Accept(Visitor<int> &visitor) {
  return visitor.Visit(shared_from_base<Postfix>());
}

Type Postfix::Accept(Visitor<Type> &visitor) {
  return visitor.Visit(shared_from_base<Postfix>());
}

int Ternary::Accept(Visitor<int>& visitor) {
  return visitor.Visit(shared_from_base<Ternary>());
}

Type Ternary::Accept(Visitor<Type>& visitor) {
  return visitor.Visit(shared_from_base<Ternary>());
}

int Index::Accept(Visitor<int>& visitor) {
  return visitor.Visit(shared_from_base<Index>());
}

Type Index::Accept(Visitor<Type>& visitor) {
  return visitor.Visit(shared_from_base<Index>());
}

int Label::Accept(Visitor<int>& visitor) {
  return visitor.Visit(shared_from_base<Label>());
}

Type Label::Accept(Visitor<Type>& visitor) {
  return visitor.Visit(shared_from_base<Label>());
}

int GoTo::Accept(Visitor<int>& visitor) {
  return visitor.Visit(shared_from_base<GoTo>());
}

Type GoTo::Accept(Visitor<Type>& visitor) {
  return visitor.Visit(shared_from_base<GoTo>());
}

} // namespace mcc