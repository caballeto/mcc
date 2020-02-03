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
class VarDecl;

class Stmt : public std::enable_shared_from_this<Stmt> {
 public:
  virtual int Accept(Visitor& visitor) = 0;
  virtual bool IsDeclaration() const { return false; }

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

class Conditional : public Stmt {
 public:
  Conditional(std::shared_ptr<Expr> condition,
              std::shared_ptr<Stmt> then_block,
              std::shared_ptr<Stmt> else_block)
      : condition_(std::move(condition)),
        then_block_(std::move(then_block)),
        else_block_(std::move(else_block))
  { }

  int Accept(Visitor& visitor) override;

  std::shared_ptr<Expr> condition_;
  std::shared_ptr<Stmt> then_block_;
  std::shared_ptr<Stmt> else_block_;
};

class While : public Stmt {
 public:
  While(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> loop_block, bool do_while)
    : condition_(std::move(condition)), loop_block_(std::move(loop_block)), do_while_(do_while)
  { }

  int Accept(Visitor& visitor) override;

  bool do_while_;
  std::shared_ptr<Expr> condition_;
  std::shared_ptr<Stmt> loop_block_;
};

class DeclList : public Stmt {
 public:
  explicit DeclList(std::vector<std::shared_ptr<VarDecl>> var_decl_list)
    : var_decl_list_(std::move(var_decl_list))
  { }

  bool IsDeclaration() const override;
  int Accept(Visitor& visitor) override;

  std::vector<std::shared_ptr<VarDecl>> var_decl_list_;
};

// #FIXME: expr-list is Stmt?
class ExprList : public Stmt {
 public:
  explicit ExprList(std::vector<std::shared_ptr<Expr>> expr_list)
    : expr_list_(std::move(expr_list))
  { }

  int Accept(Visitor& visitor) override;

  std::vector<std::shared_ptr<Expr>> expr_list_;
};

// break/continue
class ControlFlow : public Stmt {
 public:
  explicit ControlFlow(bool is_break)
    : is_break_(is_break)
  { }

  int Accept(Visitor& visitor) override;

  bool is_break_;
};

// #TODO: add type in future
class VarDecl : public Stmt {
 public:
  explicit VarDecl(std::string name, std::shared_ptr<Expr> init, int type_id)
      : name_(std::move(name)), init_(std::move(init)), type_id_(type_id)
  { }

  int Accept(Visitor& visitor) override;
  bool IsDeclaration() const override;

  std::string name_;
  std::shared_ptr<Expr> init_;
  int type_id_;
};

class For : public Stmt {
 public:
  For(std::shared_ptr<Stmt> init, std::shared_ptr<Expr> condition,
      std::shared_ptr<Stmt> update, std::shared_ptr<Stmt> loop_block)
    : init_(std::move(init)), condition_(std::move(condition)),
      update_(std::move(update)), loop_block_(std::move(loop_block))
  { }

  int Accept(Visitor& visitor) override;

  std::shared_ptr<Stmt> init_;
  std::shared_ptr<Expr> condition_;
  std::shared_ptr<Stmt> update_;
  std::shared_ptr<Stmt> loop_block_;
};

class ExpressionStmt : public Stmt {
 public:
  explicit ExpressionStmt(std::shared_ptr<Expr> expr)
    : expr_(std::move(expr))
  { }

  int Accept(Visitor& visitor) override;

  std::shared_ptr<Expr> expr_;
};

// #FIXME: pointer indirection
class Expr : public std::enable_shared_from_this<Expr> {
 public:
  virtual int Accept(Visitor& visitor) = 0;
  virtual bool IsVariable();

  template <typename T>
  std::shared_ptr<T> shared_from_base() {
    return std::static_pointer_cast<T>(shared_from_this());
  }

  TokenType type_;
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
