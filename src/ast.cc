//
// Created by vladyslav on 15.01.20.
//

#include "ast.h"
#include "Visitor.h"

namespace mcc {

void Binary::Accept(Visitor<void> &visitor) {
  visitor.Visit(*this);
}

int Binary::Accept(Visitor<int> &visitor) {
  return visitor.Visit(*this);
}

long Binary::Accept(Visitor<long> &visitor) {
  return visitor.Visit(*this);
}

void Literal::Accept(Visitor<void> &visitor) {
  visitor.Visit(*this);
}

int Literal::Accept(Visitor<int> &visitor) {
  return visitor.Visit(*this);
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
long Literal::Accept(Visitor<long> &visitor) {
  return visitor.Visit(*this);
}

void Print::Accept(Visitor<void> &visitor) {
  visitor.Visit(*this);
}

int Print::Accept(Visitor<int> &visitor) {
  return visitor.Visit(*this);
}
long Print::Accept(Visitor<long> &visitor) {
  return visitor.Visit(*this);
}

void Assign::Accept(Visitor<void> &visitor) {
  visitor.Visit(*this);
}

int Assign::Accept(Visitor<int> &visitor) {
  return visitor.Visit(*this);
}
long Assign::Accept(Visitor<long> &visitor) {
  return visitor.Visit(*this);
}

void VarDecl::Accept(Visitor<void> &visitor) {
  visitor.Visit(*this);
}

int VarDecl::Accept(Visitor<int> &visitor) {
  return visitor.Visit(*this);
}

bool VarDecl::IsDeclaration() const {
  return true;
}
long VarDecl::Accept(Visitor<long> &visitor) {
  return visitor.Visit(*this);
}

void ExpressionStmt::Accept(Visitor<void> &visitor) {
  visitor.Visit(*this);
}

int ExpressionStmt::Accept(Visitor<int> &visitor) {
  return visitor.Visit(*this);
}
long ExpressionStmt::Accept(Visitor<long> &visitor) {
  return visitor.Visit(*this);
}

void Conditional::Accept(Visitor<void> &visitor) {
  visitor.Visit(*this);
}

int Conditional::Accept(Visitor<int> &visitor) {
  return visitor.Visit(*this);
}

long Conditional::Accept(Visitor<long> &visitor) {
  return visitor.Visit(*this);
}

void Block::Accept(Visitor<void> &visitor) {
  visitor.Visit(*this);
}

int Block::Accept(Visitor<int> &visitor) {
  return visitor.Visit(*this);
}
long Block::Accept(Visitor<long> &visitor) {
  return visitor.Visit(*this);
}

void While::Accept(Visitor<void> &visitor) {
  visitor.Visit(*this);
}

int While::Accept(Visitor<int> &visitor) {
  return visitor.Visit(*this);
}
long While::Accept(Visitor<long> &visitor) {
  return visitor.Visit(*this);
}

void For::Accept(Visitor<void> &visitor) {
  visitor.Visit(*this);
}

int For::Accept(Visitor<int> &visitor) {
  return visitor.Visit(*this);
}
long For::Accept(Visitor<long> &visitor) {
  return visitor.Visit(*this);
}

void DeclList::Accept(Visitor<void> &visitor) {
  visitor.Visit(*this);
}

int DeclList::Accept(Visitor<int> &visitor) {
  return visitor.Visit(*this);
}

bool DeclList::IsDeclaration() const {
  return true;
}
long DeclList::Accept(Visitor<long> &visitor) {
  return visitor.Visit(*this);
}

void ExprList::Accept(Visitor<void> &visitor) {
  visitor.Visit(*this);
}

int ExprList::Accept(Visitor<int> &visitor) {
  return visitor.Visit(*this);
}
long ExprList::Accept(Visitor<long> &visitor) {
  return visitor.Visit(*this);
}

void ControlFlow::Accept(Visitor<void> &visitor) {
   visitor.Visit(*this);
}

int ControlFlow::Accept(Visitor<int> &visitor) {
  return visitor.Visit(*this);
}

long ControlFlow::Accept(Visitor<long> &visitor) {
  return visitor.Visit(*this);
}

void Unary::Accept(Visitor<void> &visitor) {
  visitor.Visit(*this);
}

int Unary::Accept(Visitor<int> &visitor) {
  return visitor.Visit(*this);
}
long Unary::Accept(Visitor<long> &visitor) {
  return visitor.Visit(*this);
}

bool FuncDecl::IsDeclaration() const {
  return true;
}

void FuncDecl::Accept(Visitor<void> &visitor) {
  visitor.Visit(*this);
}

int FuncDecl::Accept(Visitor<int> &visitor) {
  return visitor.Visit(*this);
}
long FuncDecl::Accept(Visitor<long> &visitor) {
  return visitor.Visit(*this);
}

int Return::Accept(Visitor<int> &visitor) {
  return visitor.Visit(*this);
}

void Return::Accept(Visitor<void> &visitor) {
  visitor.Visit(*this);
}
long Return::Accept(Visitor<long> &visitor) {
  return visitor.Visit(*this);
}

int Call::Accept(Visitor<int> &visitor) {
  return visitor.Visit(*this);
}

void Call::Accept(Visitor<void> &visitor) {
  visitor.Visit(*this);
}
long Call::Accept(Visitor<long> &visitor) {
  return visitor.Visit(*this);
}

int Grouping::Accept(Visitor<int> &visitor) {
  return visitor.Visit(*this);
}

void Grouping::Accept(Visitor<void> &visitor) {
  visitor.Visit(*this);
}
long Grouping::Accept(Visitor<long> &visitor) {
  return visitor.Visit(*this);
}

int Postfix::Accept(Visitor<int> &visitor) {
  return visitor.Visit(*this);
}

void Postfix::Accept(Visitor<void> &visitor) {
  visitor.Visit(*this);
}
long Postfix::Accept(Visitor<long> &visitor) {
  return visitor.Visit(*this);
}

int Ternary::Accept(Visitor<int>& visitor) {
  return visitor.Visit(*this);
}

void Ternary::Accept(Visitor<void>& visitor) {
  visitor.Visit(*this);
}
long Ternary::Accept(Visitor<long> &visitor) {
  return visitor.Visit(*this);
}

int Index::Accept(Visitor<int>& visitor) {
  return visitor.Visit(*this);
}

void Index::Accept(Visitor<void>& visitor) {
  visitor.Visit(*this);
}
long Index::Accept(Visitor<long> &visitor) {
  return visitor.Visit(*this);
}

int Label::Accept(Visitor<int>& visitor) {
  return visitor.Visit(*this);
}

void Label::Accept(Visitor<void>& visitor) {
  visitor.Visit(*this);
}
long Label::Accept(Visitor<long> &visitor) {
  return visitor.Visit(*this);
}

int GoTo::Accept(Visitor<int>& visitor) {
  return visitor.Visit(*this);
}

void GoTo::Accept(Visitor<void>& visitor) {
  visitor.Visit(*this);
}
long GoTo::Accept(Visitor<long> &visitor) {
  return visitor.Visit(*this);
}

int Struct::Accept(Visitor<int> &visitor) {
  return visitor.Visit(*this);
}

void Struct::Accept(Visitor<void> &visitor) {
  visitor.Visit(*this);
}

bool Struct::IsStruct() const {
  return true;
}
long Struct::Accept(Visitor<long> &visitor) {
  return visitor.Visit(*this);
}

int Access::Accept(Visitor<int> &visitor) {
  return visitor.Visit(*this);
}

void Access::Accept(Visitor<void> &visitor) {
  visitor.Visit(*this);
}
long Access::Accept(Visitor<long> &visitor) {
  return visitor.Visit(*this);
}

int Union::Accept(Visitor<int> &visitor) {
  return visitor.Visit(*this);;
}

void Union::Accept(Visitor<void> &visitor) {
  visitor.Visit(*this);
}

bool Union::IsUnion() const {
  return true;
}
long Union::Accept(Visitor<long> &visitor) {
  return visitor.Visit(*this);
}

int Enum::Accept(Visitor<int> &visitor) {
  return visitor.Visit(*this);;
}

void Enum::Accept(Visitor<void> &visitor) {
  visitor.Visit(*this);
}
long Enum::Accept(Visitor<long> &visitor) {
  return visitor.Visit(*this);
}

int Switch::Accept(Visitor<int> &visitor) {
  return visitor.Visit(*this);
}

void Switch::Accept(Visitor<void> &visitor) {
  visitor.Visit(*this);
}

long Switch::Accept(Visitor<long> &visitor) {
  return visitor.Visit(*this);
}

int Typedef::Accept(Visitor<int> &visitor) {
  return visitor.Visit(*this);
}

void Typedef::Accept(Visitor<void> &visitor) {
  visitor.Visit(*this);
}

int TypeCast::Accept(Visitor<int> &visitor) {
  return visitor.Visit(*this);
}

void TypeCast::Accept(Visitor<void> &visitor) {
  visitor.Visit(*this);
}
long TypeCast::Accept(Visitor<long> &visitor) {
  return visitor.Visit(*this);
}

long Typedef::Accept(Visitor<long> &visitor) {
  return visitor.Visit(*this);
}

int Logical::Accept(Visitor<int> &visitor) {
  return visitor.Visit(*this);
}

long Logical::Accept(Visitor<long> &visitor) {
  return visitor.Visit(*this);
}

void Logical::Accept(Visitor<void> &visitor) {
  visitor.Visit(*this);
}

} // namespace mcc