//
// Created by vladyslav on 15.01.20.
//

#ifndef MCC_SRC_AST_H_
#define MCC_SRC_AST_H_

#include <utility>

#include "common.h"
#include "Token.h"
#include "Type.h"
#include "SymbolTable.h"

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
  virtual long Accept(Visitor<long>& visitor) = 0;
  virtual void Accept(Visitor<void>& visitor) = 0;
  virtual bool IsDeclaration() const { return false; }
  virtual bool IsUnion() const { return false; }
  virtual bool IsStruct() const { return false; }

  template <typename T>
  std::shared_ptr<T> shared_from_base() {
    return std::static_pointer_cast<T>(shared_from_this());
  }

  std::shared_ptr<Token> token_;
};

class Typedef : public Stmt {
 public:
  Typedef(std::shared_ptr<Token> token, const Type& type, std::shared_ptr<Token> name, std::shared_ptr<Stmt> stmt)
    : Stmt(std::move(token)), type_(type), name_(std::move(name)), stmt_(std::move(stmt))
  { }

  long Accept(Visitor<long>& visitor) override;
  int Accept(Visitor<int>& visitor) override;
  void Accept(Visitor<void>& visitor) override;

  Type type_;
  std::shared_ptr<Stmt> stmt_;
  std::shared_ptr<Token> name_; // typedef name
};

class Switch : public Stmt {
 public:
  Switch(std::shared_ptr<Token> token, std::shared_ptr<Expr> expr, std::vector<std::pair<std::shared_ptr<Expr>, std::shared_ptr<Stmt>>> cases)
    : Stmt(std::move(token)), expr_(std::move(expr)), cases_(std::move(cases))
  { }

  long Accept(Visitor<long>& visitor) override;
  int Accept(Visitor<int>& visitor) override;
  void Accept(Visitor<void>& visitor) override;

  std::shared_ptr<Expr> expr_; // switch expr
  std::vector<std::pair<std::shared_ptr<Expr>, std::shared_ptr<Stmt>>> cases_; // case & default branches
};

class Block : public Stmt {
 public:
  Block(std::shared_ptr<Token> token, std::vector<std::shared_ptr<Stmt>> stmts)
    : Stmt(std::move(token)), stmts_(std::move(stmts))
  { }

  long Accept(Visitor<long>& visitor) override;
  int Accept(Visitor<int>& visitor) override;
  void Accept(Visitor<void>& visitor) override;

  std::vector<std::shared_ptr<Stmt>> stmts_;
};

class Print : public Stmt {
 public:
  Print(std::shared_ptr<Token> token, std::shared_ptr<Expr> expr)
    : Stmt(std::move(token)), expr_(std::move(expr))
  { }

  long Accept(Visitor<long>& visitor) override;
  int Accept(Visitor<int>& visitor) override;
  void Accept(Visitor<void>& visitor) override;

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

  long Accept(Visitor<long>& visitor) override;
  int Accept(Visitor<int>& visitor) override;
  void Accept(Visitor<void>& visitor) override;

  std::shared_ptr<Expr> condition_;
  std::shared_ptr<Stmt> then_block_;
  std::shared_ptr<Stmt> else_block_;
};

class While : public Stmt {
 public:
  While(std::shared_ptr<Token> token, std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> loop_block, bool do_while)
    : Stmt(std::move(token)), condition_(std::move(condition)), loop_block_(std::move(loop_block)), do_while_(do_while)
  { }

  long Accept(Visitor<long>& visitor) override;
  int Accept(Visitor<int>& visitor) override;
  void Accept(Visitor<void>& visitor) override;

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

  long Accept(Visitor<long>& visitor) override;
  int Accept(Visitor<int>& visitor) override;
  void Accept(Visitor<void>& visitor) override;

  std::vector<std::shared_ptr<VarDecl>> var_decl_list_;
};

// #FIXME: expr-list is Stmt?
class ExprList : public Stmt {
 public:
  ExprList(std::shared_ptr<Token> token, std::vector<std::shared_ptr<Expr>> expr_list)
    : Stmt(std::move(token)), expr_list_(std::move(expr_list))
  { }

  long Accept(Visitor<long>& visitor) override;
  int Accept(Visitor<int>& visitor) override;
  void Accept(Visitor<void>& visitor) override;

  std::vector<std::shared_ptr<Expr>> expr_list_;
};

class Return : public Stmt {
 public:
  explicit Return(std::shared_ptr<Token> token, std::shared_ptr<Expr> expr)
    : Stmt(std::move(token)), expr_(std::move(expr))
  { }

  long Accept(Visitor<long>& visitor) override;
  int Accept(Visitor<int>& visitor) override;
  void Accept(Visitor<void>& visitor) override;

  std::shared_ptr<Expr> expr_;
};

// break/continue
class ControlFlow : public Stmt {
 public:
  ControlFlow(std::shared_ptr<Token> token, bool is_break)
    : Stmt(std::move(token)), is_break_(is_break)
  { }

  long Accept(Visitor<long>& visitor) override;
  int Accept(Visitor<int>& visitor) override;
  void Accept(Visitor<void>& visitor) override;

  bool is_break_;
};


class Struct : public Stmt {
 public:
  Struct(std::shared_ptr<Token> token, const Type& type, std::shared_ptr<DeclList> body, std::shared_ptr<Token> var_name)
    : Stmt(std::move(token)), type_(type), body_(std::move(body)), var_name_(std::move(var_name))
  { }

  long Accept(Visitor<long>& visitor) override;
  int Accept(Visitor<int>& visitor) override;
  void Accept(Visitor<void>& visitor) override;

  bool IsStruct() const override;

  bool is_typedef_ = false;
  Entry *fields_;
  Type type_;
  std::shared_ptr<Token> var_name_;
  std::shared_ptr<DeclList> body_;
  int size;
};

class Union : public Stmt {
 public:
  Union(std::shared_ptr<Token> token, const Type& type, std::shared_ptr<DeclList> body, std::shared_ptr<Token> var_name)
  : Stmt(std::move(token)), type_(type), body_(std::move(body)), var_name_(std::move(var_name))
  { }

  long Accept(Visitor<long>& visitor) override;
  int Accept(Visitor<int>& visitor) override;
  void Accept(Visitor<void>& visitor) override;

  bool IsUnion() const override;

  bool is_typedef_ = false;
  Entry *fields_;
  Type type_;
  std::shared_ptr<Token> var_name_;
  std::shared_ptr<DeclList> body_;
  int size;
};

class Enum : public Stmt {
 public:
  Enum(std::shared_ptr<Token> token, const Type& type, std::shared_ptr<Token> var_name, std::vector<std::shared_ptr<Literal>> values)
    : Stmt(std::move(token)), type_(type), var_name_(std::move(var_name)), values_(std::move(values))
  { }

  long Accept(Visitor<long>& visitor) override;
  int Accept(Visitor<int>& visitor) override;
  void Accept(Visitor<void>& visitor) override;

  Type type_;
  std::shared_ptr<Token> var_name_;
  std::vector<std::shared_ptr<Literal>> values_;
};

class FuncDecl : public Stmt {
 public:
  FuncDecl(
      Type return_type,
      std::shared_ptr<Token> name,
      std::shared_ptr<DeclList> signature,
      std::shared_ptr<Block> body)
    : Stmt(name),
    return_type_(std::move(return_type)),
    name_(std::move(name)),
    signature_(std::move(signature)),
    body_(std::move(body))
  { }

  long Accept(Visitor<long>& visitor) override;
  int Accept(Visitor<int>& visitor) override;
  void Accept(Visitor<void>& visitor) override;
  bool IsDeclaration() const override;

  Type return_type_;

  int local_offset_;
  std::shared_ptr<Token> name_;
  std::shared_ptr<DeclList> signature_;
  std::shared_ptr<Block> body_;
};

class VarDecl : public Stmt {
 public:
  VarDecl(
      std::shared_ptr<Token> name,
      std::shared_ptr<Expr> init,
      Type var_type,
      bool is_const_init,
      bool is_local)
  : Stmt(nullptr),
    name_(std::move(name)),
    init_(std::move(init)),
    var_type_(std::move(var_type)),
    is_const_init_(is_const_init),
    is_local_(is_local),
    init_val_(0)
  { }

  VarDecl(
      std::shared_ptr<Token> name,
      Type var_type,
      bool is_local)
      : Stmt(nullptr),
        name_(std::move(name)),
        init_(nullptr),
        var_type_(std::move(var_type)),
        is_const_init_(false),
        is_local_(is_local),
        init_val_(0)
  { }

  long Accept(Visitor<long>& visitor) override;
  int Accept(Visitor<int>& visitor) override;
  void Accept(Visitor<void>& visitor) override;

  bool IsDeclaration() const override;

  std::shared_ptr<Token> name_;
  std::shared_ptr<Expr> init_;

  Type var_type_;

  long init_val_;
  int offset_;
  bool is_const_init_;
  bool is_local_;
};

class For : public Stmt {
 public:
  For(std::shared_ptr<Token> token, std::shared_ptr<Stmt> init, std::shared_ptr<Expr> condition,
      std::shared_ptr<Stmt> update, std::shared_ptr<Stmt> loop_block)
    : Stmt(std::move(token)), init_(std::move(init)), condition_(std::move(condition)),
      update_(std::move(update)), loop_block_(std::move(loop_block))
  { }

  long Accept(Visitor<long>& visitor) override;
  int Accept(Visitor<int>& visitor) override;
  void Accept(Visitor<void>& visitor) override;

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

  long Accept(Visitor<long>& visitor) override;
  int Accept(Visitor<int>& visitor) override;
  void Accept(Visitor<void>& visitor) override;

  std::shared_ptr<Expr> expr_;
};

class Expr : public std::enable_shared_from_this<Expr> {
 public:
  explicit Expr(std::shared_ptr<Token> op)
    : op_(std::move(op)), offset_(0)
  { }

  inline TokenType type() { return type_.type_; }
  inline int ind() { return type_.ind; }

  virtual long Accept(Visitor<long>& visitor) = 0;
  virtual int Accept(Visitor<int>& visitor) = 0;
  virtual void Accept(Visitor<void>& visitor) = 0;

  virtual bool IsVariable();
  virtual bool IsLvalue();
  virtual bool IsIntConstant(); // check if integer compile-time constant
  virtual bool IsCompileConstant(); // check if compile-time constant (int, string)

  template <typename T>
  std::shared_ptr<T> shared_from_base() {
    return std::static_pointer_cast<T>(shared_from_this());
  }

  // TODO: rewrite flags as bitfields
  Type type_;

  int offset_;

  bool is_lvalue_ = false;
  bool is_const_ = false;
  bool is_function_ = false;
  bool is_local_ = false;
  bool is_indexable_ = false;
  bool to_scale_ = false;
  bool return_ptr_ = false;

  std::shared_ptr<Token> op_;
};

class TypeCast : public Expr {
 public:
  TypeCast(std::shared_ptr<Token> op, std::shared_ptr<Expr> expr)
    : Expr(std::move(op)), expr_(std::move(expr))
  { }

  long Accept(Visitor<long>& visitor) override;
  int Accept(Visitor<int>& visitor) override;
  void Accept(Visitor<void>& visitor) override;

  std::shared_ptr<Expr> expr_;
};

class Index : public Expr {
 public:
  Index(std::shared_ptr<Token> op, std::shared_ptr<Expr> name, std::shared_ptr<Expr> index)
    : Expr(std::move(op)), name_(std::move(name)), index_(std::move(index))
  { }

  long Accept(Visitor<long>& visitor) override;
  int Accept(Visitor<int>& visitor) override;
  void Accept(Visitor<void>& visitor) override;

  std::shared_ptr<Expr> name_;
  std::shared_ptr<Expr> index_;
};

class Access : public Expr {
 public:
  Access(std::shared_ptr<Token> op, std::shared_ptr<Expr> name, std::shared_ptr<Expr> field)
    : Expr(std::move(op)), name_(std::move(name)), field_(std::move(field))
  { }

  long Accept(Visitor<long>& visitor) override;
  int Accept(Visitor<int>& visitor) override;
  void Accept(Visitor<void>& visitor) override;

  std::shared_ptr<Expr> name_;
  std::shared_ptr<Expr> field_;
};

class Label : public Stmt {
 public:
  explicit Label(std::shared_ptr<Token> name)
    : Stmt(std::move(name)), label_(-1)
  { }

  long Accept(Visitor<long>& visitor) override;
  int Accept(Visitor<int>& visitor) override;
  void Accept(Visitor<void>& visitor) override;

  int label_{};
};

class GoTo : public Stmt {
 public:
  explicit GoTo(std::shared_ptr<Token> name)
    : Stmt(std::move(name)), label_(-1)
  { }

  long Accept(Visitor<long>& visitor) override;
  int Accept(Visitor<int>& visitor) override;
  void Accept(Visitor<void>& visitor) override;

  int label_;
};

class Call : public Expr {
 public:
  Call(std::shared_ptr<Token> op, std::shared_ptr<Expr> name, std::shared_ptr<ExprList> args)
    : Expr(std::move(op)), name_(std::move(name)), args_(std::move(args))
  { }

  long Accept(Visitor<long>& visitor) override;
  int Accept(Visitor<int>& visitor) override;
  void Accept(Visitor<void>& visitor) override;

  std::shared_ptr<Expr> name_;
  std::shared_ptr<ExprList> args_;
};

class Assign : public Expr {
 public:
  Assign(std::shared_ptr<Token> op, std::shared_ptr<Expr> left, std::shared_ptr<Expr> right)
    : Expr(std::move(op)), left_(std::move(left)), right_(std::move(right))
  { }

  long Accept(Visitor<long>& visitor) override;
  int Accept(Visitor<int>& visitor) override;
  void Accept(Visitor<void>& visitor) override;

  std::shared_ptr<Expr> left_;
  std::shared_ptr<Expr> right_;
};

class Ternary : public Expr {
 public:
  Ternary(
      std::shared_ptr<Token> op,
      std::shared_ptr<Expr> condition,
      std::shared_ptr<Expr> then,
      std::shared_ptr<Expr> else_branch)
    : Expr(std::move(op)),
    condition_(std::move(condition)),
    then_(std::move(then)),
    else_(std::move(else_branch))
  { }

  long Accept(Visitor<long>& visitor) override;
  int Accept(Visitor<int>& visitor) override;
  void Accept(Visitor<void>& visitor) override;

  std::shared_ptr<Expr> condition_;
  std::shared_ptr<Expr> then_;
  std::shared_ptr<Expr> else_;
};

class Postfix : public Expr {
 public:
  Postfix(std::shared_ptr<Token> op, std::shared_ptr<Expr> expr)
    : Expr(std::move(op)), expr_(std::move(expr))
  { }

  long Accept(Visitor<long>& visitor) override;
  int Accept(Visitor<int>& visitor) override;
  void Accept(Visitor<void>& visitor) override;

  std::shared_ptr<Expr> expr_;
};

class Grouping : public Expr {
 public:
  Grouping(std::shared_ptr<Token> op, std::shared_ptr<Expr> expr)
    : Expr(std::move(op)), expr_(std::move(expr))
  { }

  long Accept(Visitor<long>& visitor) override;
  int Accept(Visitor<int>& visitor) override;
  void Accept(Visitor<void>& visitor) override;

  std::shared_ptr<Expr> expr_;
};

class Binary : public Expr {
 public:
  Binary(std::shared_ptr<Token> op, std::shared_ptr<Expr> left, std::shared_ptr<Expr> right)
    : Expr(std::move(op)), left_(std::move(left)), right_(std::move(right))
  { }

  long Accept(Visitor<long>& visitor) override;
  int Accept(Visitor<int>& visitor) override;
  void Accept(Visitor<void>& visitor) override;

  std::shared_ptr<Expr> left_;
  std::shared_ptr<Expr> right_;
};

// #FIXME: override is variable
class Unary : public Expr {
 public:
  Unary(std::shared_ptr<Token> op, std::shared_ptr<Expr> expr)
    : Expr(std::move(op)), expr_(std::move(expr))
  { }

  long Accept(Visitor<long>& visitor) override;
  int Accept(Visitor<int>& visitor) override;
  void Accept(Visitor<void>& visitor) override;

  std::shared_ptr<Expr> expr_;
};

class Literal : public Expr {
 public:
  explicit Literal(std::shared_ptr<Token> op)
    : Expr(std::move(op))
  { }

  long Accept(Visitor<long>& visitor) override;
  int Accept(Visitor<int>& visitor) override;
  void Accept(Visitor<void>& visitor) override;

  bool IsIntConstant() override;
  bool IsCompileConstant() override;
  bool IsVariable() override;
};

} // namespace mcc

#endif //MCC_SRC_AST_H_
