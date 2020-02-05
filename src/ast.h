//
// Created by vladyslav on 15.01.20.
//

#ifndef MCC_SRC_AST_H_
#define MCC_SRC_AST_H_

#include <utility>

#include "common.h"
#include "Token.h"
#include "Type.h"

namespace mcc {

template <typename T>
class Visitor;

class Block;
class Expr;
class Literal;
class VarDecl;

class Stmt : public std::enable_shared_from_this<Stmt> {
 public:
  explicit Stmt(std::shared_ptr<Token> token)
    : token_(std::move(token))
  { }

  virtual int Accept(Visitor<int>& visitor) = 0;
  virtual Type Accept(Visitor<Type>& visitor) = 0;
  virtual bool IsDeclaration() const { return false; }

  template <typename T>
  std::shared_ptr<T> shared_from_base() {
    return std::static_pointer_cast<T>(shared_from_this());
  }

  std::shared_ptr<Token> token_;
};

class Block : public Stmt {
 public:
  Block(std::shared_ptr<Token> token, std::vector<std::shared_ptr<Stmt>> stmts)
    : Stmt(std::move(token)), stmts_(std::move(stmts))
  { }

  int Accept(Visitor<int>& visitor) override;
  Type Accept(Visitor<Type>& visitor) override;

  std::vector<std::shared_ptr<Stmt>> stmts_;
};

class Print : public Stmt {
 public:
  Print(std::shared_ptr<Token> token, std::shared_ptr<Expr> expr)
    : Stmt(std::move(token)), expr_(std::move(expr))
  { }

  int Accept(Visitor<int>& visitor) override;
  Type Accept(Visitor<Type>& visitor) override;

  std::shared_ptr<Expr> expr_;
};

class Conditional : public Stmt {
 public:
  Conditional(std::shared_ptr<Token> token,
              std::shared_ptr<Expr> condition,
              std::shared_ptr<Stmt> then_block,
              std::shared_ptr<Stmt> else_block)
      : Stmt(std::move(token)),
        condition_(std::move(condition)),
        then_block_(std::move(then_block)),
        else_block_(std::move(else_block))
  { }

  int Accept(Visitor<int>& visitor) override;
  Type Accept(Visitor<Type>& visitor) override;

  std::shared_ptr<Expr> condition_;
  std::shared_ptr<Stmt> then_block_;
  std::shared_ptr<Stmt> else_block_;
};

class While : public Stmt {
 public:
  While(std::shared_ptr<Token> token, std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> loop_block, bool do_while)
    : Stmt(std::move(token)), condition_(std::move(condition)), loop_block_(std::move(loop_block)), do_while_(do_while)
  { }

  int Accept(Visitor<int>& visitor) override;
  Type Accept(Visitor<Type>& visitor) override;

  bool do_while_;
  std::shared_ptr<Expr> condition_;
  std::shared_ptr<Stmt> loop_block_;
};

class DeclList : public Stmt {
 public:
  DeclList(std::shared_ptr<Token> token, std::vector<std::shared_ptr<VarDecl>> var_decl_list)
    : Stmt(std::move(token)), var_decl_list_(std::move(var_decl_list))
  { }

  bool IsDeclaration() const override;
  int Accept(Visitor<int>& visitor) override;
  Type Accept(Visitor<Type>& visitor) override;

  std::vector<std::shared_ptr<VarDecl>> var_decl_list_;
};

// #FIXME: expr-list is Stmt?
class ExprList : public Stmt {
 public:
  ExprList(std::shared_ptr<Token> token, std::vector<std::shared_ptr<Expr>> expr_list)
    : Stmt(std::move(token)), expr_list_(std::move(expr_list))
  { }

  int Accept(Visitor<int>& visitor) override;
  Type Accept(Visitor<Type>& visitor) override;

  std::vector<std::shared_ptr<Expr>> expr_list_;
};

// break/continue
class ControlFlow : public Stmt {
 public:
  ControlFlow(std::shared_ptr<Token> token, bool is_break)
    : Stmt(std::move(token)), is_break_(is_break)
  { }

  int Accept(Visitor<int>& visitor) override;
  Type Accept(Visitor<Type>& visitor) override;

  bool is_break_;
};

// #TODO: add type in future
class VarDecl : public Stmt {
 public:
  VarDecl(std::shared_ptr<Token> token, std::shared_ptr<Token> name, std::shared_ptr<Expr> init, Type var_type, int indirection)
      : Stmt(std::move(token)), name_(std::move(name)), init_(std::move(init)), var_type_(var_type), indirection_(indirection)
  { }

  int Accept(Visitor<int>& visitor) override;
  Type Accept(Visitor<Type>& visitor) override;

  bool IsDeclaration() const override;

  std::shared_ptr<Token> name_;
  std::shared_ptr<Expr> init_;
  Type var_type_;
  int indirection_;
};

class For : public Stmt {
 public:
  For(std::shared_ptr<Token> token, std::shared_ptr<Stmt> init, std::shared_ptr<Expr> condition,
      std::shared_ptr<Stmt> update, std::shared_ptr<Stmt> loop_block)
    : Stmt(std::move(token)), init_(std::move(init)), condition_(std::move(condition)),
      update_(std::move(update)), loop_block_(std::move(loop_block))
  { }

  int Accept(Visitor<int>& visitor) override;
  Type Accept(Visitor<Type>& visitor) override;

  std::shared_ptr<Stmt> init_;
  std::shared_ptr<Expr> condition_;
  std::shared_ptr<Stmt> update_;
  std::shared_ptr<Stmt> loop_block_;
};

class ExpressionStmt : public Stmt {
 public:
  explicit ExpressionStmt(std::shared_ptr<Token> token, std::shared_ptr<Expr> expr)
    : Stmt(std::move(token)), expr_(std::move(expr))
  { }

  int Accept(Visitor<int>& visitor) override;
  Type Accept(Visitor<Type>& visitor) override;

  std::shared_ptr<Expr> expr_;
};

class Expr : public std::enable_shared_from_this<Expr> {
 public:
  explicit Expr(std::shared_ptr<Token> op)
    : op_(std::move(op))
  { }

  virtual int Accept(Visitor<int>& visitor) = 0;
  virtual Type Accept(Visitor<Type>& visitor) = 0;

  virtual bool IsVariable();
  virtual bool IsLvalue();

  template <typename T>
  std::shared_ptr<T> shared_from_base() {
    return std::static_pointer_cast<T>(shared_from_this());
  }

  Type type_ = Type::NONE;
  int indirection_ = 0;
  bool is_lvalue = false;
  std::shared_ptr<Token> op_;
};

class Assign : public Expr {
 public:
  Assign(std::shared_ptr<Token> op, std::shared_ptr<Expr> left, std::shared_ptr<Expr> right)
    : Expr(std::move(op)), left_(std::move(left)), right_(std::move(right))
  { }

  int Accept(Visitor<int>& visitor) override;
  Type Accept(Visitor<Type>& visitor) override;

  std::shared_ptr<Expr> left_;
  std::shared_ptr<Expr> right_;
};

class Binary : public Expr {
 public:
  Binary(std::shared_ptr<Token> op, std::shared_ptr<Expr> left, std::shared_ptr<Expr> right)
    : Expr(std::move(op)), left_(std::move(left)), right_(std::move(right))
  { }

  int Accept(Visitor<int>& visitor) override;
  Type Accept(Visitor<Type>& visitor) override;

  std::shared_ptr<Expr> left_;
  std::shared_ptr<Expr> right_;
};

// #FIXME: override is variable
class Unary : public Expr {
 public:
  Unary(std::shared_ptr<Token> op, std::shared_ptr<Expr> right)
    : Expr(std::move(op)), right_(std::move(right))
  { }

  int Accept(Visitor<int>& visitor) override;
  Type Accept(Visitor<Type>& visitor) override;

  std::shared_ptr<Expr> right_;
};

class Literal : public Expr {
 public:
  explicit Literal(std::shared_ptr<Token> op)
    : Expr(std::move(op))
  { }

  int Accept(Visitor<int>& visitor) override;
  Type Accept(Visitor<Type>& visitor) override;

  bool IsVariable() override;
};

} // namespace mcc

#endif //MCC_SRC_AST_H_
