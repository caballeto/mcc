//
// Created by vladyslav on 15.01.20.
//

#include "ast.h"
#include "Visitor.h"

namespace mcc {

void Binary::Accept(Visitor<void> &visitor) {
  visitor.Visit(shared_from_base<Binary>());
}

int Binary::Accept(Visitor<int> &visitor) {
  return visitor.Visit(shared_from_base<Binary>());
}

void Literal::Accept(Visitor<void> &visitor) {
  visitor.Visit(shared_from_base<Literal>());
}

int Literal::Accept(Visitor<int> &visitor) {
  return visitor.Visit(shared_from_base<Literal>());
}

bool Expr::IsCompileConstant() {
  return false;
}

bool Expr::IsIntConstant() {
  return false;
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

bool Literal::IsIntConstant() {
  return op_->GetType() == TokenType::T_INT_LIT;
}

bool Literal::IsCompileConstant() {
  return op_->GetType() == TokenType::T_INT_LIT || op_->GetType() == TokenType::T_STR_LIT;
}

void Print::Accept(Visitor<void> &visitor) {
  visitor.Visit(shared_from_base<Print>());
}

int Print::Accept(Visitor<int> &visitor) {
  return visitor.Visit(shared_from_base<Print>());
}

void Assign::Accept(Visitor<void> &visitor) {
  visitor.Visit(shared_from_base<Assign>());
}

int Assign::Accept(Visitor<int> &visitor) {
  return visitor.Visit(shared_from_base<Assign>());
}

void VarDecl::Accept(Visitor<void> &visitor) {
  visitor.Visit(shared_from_base<VarDecl>());
}

int VarDecl::Accept(Visitor<int> &visitor) {
  return visitor.Visit(shared_from_base<VarDecl>());
}

bool VarDecl::IsDeclaration() const {
  return true;
}

void ExpressionStmt::Accept(Visitor<void> &visitor) {
  visitor.Visit(shared_from_base<ExpressionStmt>());
}

int ExpressionStmt::Accept(Visitor<int> &visitor) {
  return visitor.Visit(shared_from_base<ExpressionStmt>());
}

void Conditional::Accept(Visitor<void> &visitor) {
  visitor.Visit(shared_from_base<Conditional>());
}

int Conditional::Accept(Visitor<int> &visitor) {
  return visitor.Visit(shared_from_base<Conditional>());
}

void Block::Accept(Visitor<void> &visitor) {
  visitor.Visit(shared_from_base<Block>());
}

int Block::Accept(Visitor<int> &visitor) {
  return visitor.Visit(shared_from_base<Block>());
}

void While::Accept(Visitor<void> &visitor) {
  visitor.Visit(shared_from_base<While>());
}

int While::Accept(Visitor<int> &visitor) {
  return visitor.Visit(shared_from_base<While>());
}

void For::Accept(Visitor<void> &visitor) {
  visitor.Visit(shared_from_base<For>());
}

int For::Accept(Visitor<int> &visitor) {
  return visitor.Visit(shared_from_base<For>());
}

void DeclList::Accept(Visitor<void> &visitor) {
  visitor.Visit(shared_from_base<DeclList>());
}

int DeclList::Accept(Visitor<int> &visitor) {
  return visitor.Visit(shared_from_base<DeclList>());
}

bool DeclList::IsDeclaration() const {
  return true;
}

void ExprList::Accept(Visitor<void> &visitor) {
  visitor.Visit(shared_from_base<ExprList>());
}

int ExprList::Accept(Visitor<int> &visitor) {
  return visitor.Visit(shared_from_base<ExprList>());
}

void ControlFlow::Accept(Visitor<void> &visitor) {
   visitor.Visit(shared_from_base<ControlFlow>());
}

int ControlFlow::Accept(Visitor<int> &visitor) {
  return visitor.Visit(shared_from_base<ControlFlow>());
}

void Unary::Accept(Visitor<void> &visitor) {
  visitor.Visit(shared_from_base<Unary>());
}

int Unary::Accept(Visitor<int> &visitor) {
  return visitor.Visit(shared_from_base<Unary>());
}

bool FuncDecl::IsDeclaration() const {
  return true;
}

void FuncDecl::Accept(Visitor<void> &visitor) {
  visitor.Visit(shared_from_base<FuncDecl>());
}

int FuncDecl::Accept(Visitor<int> &visitor) {
  return visitor.Visit(shared_from_base<FuncDecl>());
}

int Return::Accept(Visitor<int> &visitor) {
  return visitor.Visit(shared_from_base<Return>());
}

void Return::Accept(Visitor<void> &visitor) {
  visitor.Visit(shared_from_base<Return>());
}

int Call::Accept(Visitor<int> &visitor) {
  return visitor.Visit(shared_from_base<Call>());
}

void Call::Accept(Visitor<void> &visitor) {
  visitor.Visit(shared_from_base<Call>());
}

int Grouping::Accept(Visitor<int> &visitor) {
  return visitor.Visit(shared_from_base<Grouping>());
}

void Grouping::Accept(Visitor<void> &visitor) {
  visitor.Visit(shared_from_base<Grouping>());
}

int Postfix::Accept(Visitor<int> &visitor) {
  return visitor.Visit(shared_from_base<Postfix>());
}

void Postfix::Accept(Visitor<void> &visitor) {
  visitor.Visit(shared_from_base<Postfix>());
}

int Ternary::Accept(Visitor<int>& visitor) {
  return visitor.Visit(shared_from_base<Ternary>());
}

void Ternary::Accept(Visitor<void>& visitor) {
  visitor.Visit(shared_from_base<Ternary>());
}

int Index::Accept(Visitor<int>& visitor) {
  return visitor.Visit(shared_from_base<Index>());
}

void Index::Accept(Visitor<void>& visitor) {
  visitor.Visit(shared_from_base<Index>());
}

int Label::Accept(Visitor<int>& visitor) {
  return visitor.Visit(shared_from_base<Label>());
}

void Label::Accept(Visitor<void>& visitor) {
  visitor.Visit(shared_from_base<Label>());
}

int GoTo::Accept(Visitor<int>& visitor) {
  return visitor.Visit(shared_from_base<GoTo>());
}

void GoTo::Accept(Visitor<void>& visitor) {
  visitor.Visit(shared_from_base<GoTo>());
}

int Struct::Accept(Visitor<int> &visitor) {
  return visitor.Visit(shared_from_base<Struct>());
}

void Struct::Accept(Visitor<void> &visitor) {
  visitor.Visit(shared_from_base<Struct>());
}

bool Struct::IsStruct() const {
  return true;
}

int Access::Accept(Visitor<int> &visitor) {
  return visitor.Visit(shared_from_base<Access>());
}

void Access::Accept(Visitor<void> &visitor) {
  visitor.Visit(shared_from_base<Access>());
}

int Union::Accept(Visitor<int> &visitor) {
  return visitor.Visit(shared_from_base<Union>());;
}

void Union::Accept(Visitor<void> &visitor) {
  visitor.Visit(shared_from_base<Union>());
}

bool Union::IsUnion() const {
  return true;
}

int Enum::Accept(Visitor<int> &visitor) {
  return visitor.Visit(shared_from_base<Enum>());;
}

void Enum::Accept(Visitor<void> &visitor) {
  visitor.Visit(shared_from_base<Enum>());
}

int Switch::Accept(Visitor<int> &visitor) {
  return visitor.Visit(shared_from_base<Switch>());
}

void Switch::Accept(Visitor<void> &visitor) {
  visitor.Visit(shared_from_base<Switch>());
}

int Typedef::Accept(Visitor<int> &visitor) {
  return visitor.Visit(shared_from_base<Typedef>());
}

void Typedef::Accept(Visitor<void> &visitor) {
  visitor.Visit(shared_from_base<Typedef>());
}

int TypeCast::Accept(Visitor<int> &visitor) {
  return visitor.Visit(shared_from_base<TypeCast>());
}

void TypeCast::Accept(Visitor<void> &visitor) {
  visitor.Visit(shared_from_base<TypeCast>());
}

} // namespace mcc