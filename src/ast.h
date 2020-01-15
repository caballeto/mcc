//
// Created by vladyslav on 15.01.20.
//

#ifndef MCC_SRC_AST_H_
#define MCC_SRC_AST_H_

#include "common.h"
#include "Token.h"

namespace mcc {

class Visitor;

class Expr : public std::enable_shared_from_this<Expr> {
 public:
  virtual void Dump(std::ostream& os, int spaces) = 0;
  virtual int Accept(Visitor& visitor) = 0;
};

class Binary : public Expr {
 public:
  Binary(std::shared_ptr<Token> op, std::shared_ptr<Expr> left, std::shared_ptr<Expr> right)
    : op_(std::move(op)), left_(std::move(left)), right_(std::move(right))
  { }

  void Dump(std::ostream& os, int spaces) override;

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

  void Dump(std::ostream& os, int spaces) override;

  int Accept(Visitor& visitor) override;

  std::shared_ptr<Token> literal_;
};

// #FIXME: circular import with visitor, solve it (Visitor in other file?)
class Visitor {
 public:
  virtual int Visit(std::shared_ptr<Binary> binary) = 0;
  virtual int Visit(std::shared_ptr<Literal> binary) = 0;
};

} // namespace mcc

#endif //MCC_SRC_AST_H_
