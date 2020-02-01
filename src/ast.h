//
// Created by vladyslav on 15.01.20.
//

#ifndef MCC_SRC_AST_H_
#define MCC_SRC_AST_H_

#include "common.h"
#include "Token.h"

namespace mcc {

class Visitor;
class Block;
class Expr;
class Literal;

class Stmt : public std::enable_shared_from_this<Stmt> {
 public:
  virtual int Accept(Visitor& visitor) = 0;

  template <typename T>
  std::shared_ptr<T> shared_from_base() {
    return std::static_pointer_cast<T>(shared_from_this());
  }
};

class Block : public Stmt {
 public:
  explicit Block(std::vector<std::shared_ptr<Stmt>> stmts)
    : stmts_(std::move(stmts))
  { }

  int Accept(Visitor& visitor) override;

  std::vector<std::shared_ptr<Stmt>> stmts_;
};

class Print : public Stmt {
 public:
  explicit Print(std::shared_ptr<Expr> expr)
    : expr_(std::move(expr))
  { }

  int Accept(Visitor& visitor) override;

  std::shared_ptr<Expr> expr_;
};

class VarDecl : public Stmt {
 public:
  explicit VarDecl(std::string name)
    : name_(std::move(name))
  { }

  int Accept(Visitor& visitor) override;

  std::string name_;
};

class Conditional : public Stmt {
 public:
  Conditional(std::shared_ptr<Expr> condition,
              std::shared_ptr<Block> then_block,
              std::shared_ptr<Block> else_block)
      : condition_(std::move(condition)),
        then_block_(std::move(then_block)),
        else_block_(std::move(else_block))
  { }

  int Accept(Visitor& visitor) override;

  std::shared_ptr<Expr> condition_;
  std::shared_ptr<Block> then_block_;
  std::shared_ptr<Block> else_block_;
};

class ExpressionStmt : public Stmt {
 public:
  explicit ExpressionStmt(std::shared_ptr<Expr> expr)
    : expr_(std::move(expr))
  { }

  int Accept(Visitor& visitor) override;

  std::shared_ptr<Expr> expr_;
};

class Expr : public std::enable_shared_from_this<Expr> {
 public:
  virtual int Accept(Visitor& visitor) = 0;
  virtual bool IsVariable();

  template <typename T>
  std::shared_ptr<T> shared_from_base() {
    return std::static_pointer_cast<T>(shared_from_this());
  }
};

class Assign : public Expr {
 public:
  Assign(std::shared_ptr<Expr> left, std::shared_ptr<Expr> right)
    : left_(std::move(left)), right_(std::move(right))
  { }

  int Accept(Visitor& visitor) override;

  std::shared_ptr<Expr> left_;
  std::shared_ptr<Expr> right_;
};

class Binary : public Expr {
 public:
  Binary(std::shared_ptr<Token> op, std::shared_ptr<Expr> left, std::shared_ptr<Expr> right)
    : op_(std::move(op)), left_(std::move(left)), right_(std::move(right))
  { }

  int Accept(Visitor& visitor) override;

  std::shared_ptr<Token> op_;
  std::shared_ptr<Expr> left_;
  std::shared_ptr<Expr> right_;
};

class Literal : public Expr {
 public:
  explicit Literal(std::shared_ptr<Token> literal)
    : literal_(std::move(literal))
  { }

  int Accept(Visitor& visitor) override;
  bool IsVariable() override;

  std::shared_ptr<Token> literal_;
};

} // namespace mcc

#endif //MCC_SRC_AST_H_
