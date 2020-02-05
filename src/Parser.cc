//
// Created by vladyslav on 15.01.20.
//

#include "Parser.h"
#include "Type.h"
#include "ParseException.h"

namespace mcc {

std::vector<std::shared_ptr<Stmt>> Parser::Parse() {
  std::vector<std::shared_ptr<Stmt>> stmts;

  while (!Check(TokenType::T_EOF)) {
    try {
      stmts.push_back(Statement());
    } catch (const ParseException& e) {
      reporter_.ReportParseError(e);
      synchronize();
    }
  }

  return std::move(stmts);
}

// #FIXME: global elements could only have CONSTANT inits (add type check + fix parser)
// #FIXME: local variables could use themselves in their own inits (will be written to some mem (e. g. addq $1, -4(%rbp))
std::shared_ptr<Stmt> Parser::Statement() {
  switch (Peek()->GetType()) {
    case TokenType::T_PRINT:return PrintStatement();
    case TokenType::T_VOID:
    case TokenType::T_INT:
    case TokenType::T_SHORT:
    case TokenType::T_LONG:     return DeclarationList();
    case TokenType::T_IF:       return IfStatement();
    case TokenType::T_WHILE:    return WhileStatement();
    case TokenType::T_DO:       return DoWhileStatement();
    case TokenType::T_FOR:      return ForStatement();
    case TokenType::T_LBRACE:   return BlockStatement();
    case TokenType::T_BREAK:    return BreakStatement();
    case TokenType::T_CONTINUE: return ContinueStatement();
    default:
      return ExpressionStatement();
  }
}

std::shared_ptr<ControlFlow> Parser::BreakStatement() {
  std::shared_ptr<Token> token = Consume(TokenType::T_BREAK);
  Consume(TokenType::T_SEMICOLON, "Expected ';' after 'break' statement");
  return std::make_shared<ControlFlow>(token, true);
}

std::shared_ptr<ControlFlow> Parser::ContinueStatement() {
  std::shared_ptr<Token> token = Consume(TokenType::T_CONTINUE);
  Consume(TokenType::T_SEMICOLON, "Expected ';' after 'continue' statement");
  return std::make_shared<ControlFlow>(token, false);
}

std::shared_ptr<Print> Parser::PrintStatement() {
  std::shared_ptr<Token> token = Consume(TokenType::T_PRINT);
  std::shared_ptr<Expr> expr = Expression(0);
  Consume(TokenType::T_SEMICOLON, "Expected ';' after 'print' statement");
  return std::make_shared<Print>(token, expr);
}

std::shared_ptr<Conditional> Parser::IfStatement() {
  std::shared_ptr<Token> if_token = Consume(TokenType::T_IF);
  Consume(TokenType::T_LPAREN, "Expected '(' after 'if'");
  std::shared_ptr<Expr> condition = Expression(0);
  Consume(TokenType::T_RPAREN, "Expected ')' after 'if' condition");
  std::shared_ptr<Stmt> then_block_ = Statement(), else_block_;
  DisallowDecl(then_block_, if_token);

  if (Check(TokenType::T_ELSE)) {
    std::shared_ptr<Token> else_token = Consume(TokenType::T_ELSE, "Expected 'else' token");
    else_block_ = Statement();
    DisallowDecl(else_block_, else_token);
  }

  return std::make_shared<Conditional>(if_token, condition, then_block_, else_block_);
}

std::shared_ptr<While> Parser::DoWhileStatement() {
  std::shared_ptr<Token> do_token = Consume(TokenType::T_DO, "Expected 'do' token");
  std::shared_ptr<Stmt> loop_block = Statement();
  DisallowDecl(loop_block, do_token);
  Consume(TokenType::T_WHILE, "Expected 'while' after 'do'");
  Consume(TokenType::T_LPAREN, "Expected '(' after 'while'");
  std::shared_ptr<Expr> condition = Expression(0);
  Consume(TokenType::T_RPAREN, "Expected ')' after 'while' condition");
  Consume(TokenType::T_SEMICOLON, "Expected closing ';' after do-while statement");
  return std::make_shared<While>(do_token, condition, loop_block, true);
}

// #FIXME: make condition expression optional by adding break/continue
std::shared_ptr<For> Parser::ForStatement() {
  std::shared_ptr<Token> for_token = Consume(TokenType::T_FOR);
  Consume(TokenType::T_LPAREN, "Expected '(' after 'for'");

  std::shared_ptr<Stmt> init_list;
  if (Check(TokenType::T_INT)) {
    init_list = DeclarationList();
  } else {
    init_list = ExpressionList();
    Consume(TokenType::T_SEMICOLON, "Expected ';' after init in for statement");
  }

  std::shared_ptr<Expr> condition = OptionalExpression(0);
  Consume(TokenType::T_SEMICOLON, "Expected ';' after condition in for statement");

  std::shared_ptr<Stmt> update_list = ExpressionList();
  Consume(TokenType::T_RPAREN, "Expected closing ')' after 'for' statement");

  std::shared_ptr<Stmt> loop_block = Statement();
  DisallowDecl(loop_block, for_token);
  return std::make_shared<For>(for_token, init_list, condition, update_list, loop_block);
}

void Parser::DisallowDecl(const std::shared_ptr<Stmt>& stmt, const std::shared_ptr<Token>& token) {
  if (stmt->IsDeclaration()) {
    throw ParseException("Declarations are not allowed in single-statement loops", token);
  }
}

std::shared_ptr<While> Parser::WhileStatement() {
  std::shared_ptr<Token> while_token = Consume(TokenType::T_WHILE);
  Consume(TokenType::T_LPAREN, "Expected '(' after 'while'");
  std::shared_ptr<Expr> condition = Expression(0);
  Consume(TokenType::T_RPAREN, "Expected ')' after 'while' condition");
  std::shared_ptr<Stmt> loop_block = Statement();
  DisallowDecl(loop_block, while_token);
  return std::make_shared<While>(while_token, condition, loop_block, false);
}

std::shared_ptr<DeclList> Parser::DeclarationList() {
  std::shared_ptr<Token> type_token = Peek();
  MatchType();
  std::vector<std::shared_ptr<VarDecl>> var_decl_list;

  do {
    std::shared_ptr<Token> name = Consume(TokenType::T_IDENTIFIER,"Expected identifier after type declaration");
    std::shared_ptr<Expr> init = OptionalExpression(0);
    var_decl_list.push_back(std::make_shared<VarDecl>(type_token, name, init, TokenToType(type_token->GetType()), 0));
  } while (Match(TokenType::T_COMMA));

  Consume(TokenType::T_SEMICOLON, "Expected ';' after variable declaration");
  return std::make_shared<DeclList>(type_token, var_decl_list);
}

std::shared_ptr<ExprList> Parser::ExpressionList() {
  std::vector<std::shared_ptr<Expr>> expr_list;

  if (!Check(TokenType::T_SEMICOLON) && !Check(TokenType::T_RPAREN)) {
    do {
      expr_list.push_back(Expression(0));
    } while (Match(TokenType::T_COMMA));
  }

  return std::make_shared<ExprList>(nullptr, expr_list);
}

std::shared_ptr<Stmt> Parser::ExpressionStatement() {
  switch (Peek()->GetType()) {
    case TokenType::T_IDENTIFIER: {
      std::shared_ptr<Expr> expr = Expression(0);
      Consume(TokenType::T_SEMICOLON, "Expected ';' after expression statement");
      return std::make_shared<ExpressionStmt>(nullptr, expr);
    }
    default: {
      throw ParseException("Only assign expressions are supported in top-level expression statements", Peek());
    }
  }
}

std::shared_ptr<Block> Parser::BlockStatement() {
  std::shared_ptr<Token> token = Consume(TokenType::T_LBRACE, "Expected '{' at start of block");
  std::vector<std::shared_ptr<Stmt>> stmts;

  while (!Check(TokenType::T_RBRACE)) {
    stmts.push_back(Statement());
  }

  Consume(TokenType::T_RBRACE, "Expected '}' after block");
  return std::make_shared<Block>(token, std::move(stmts));
}

// Read about named local return value optimization and copy-elision
std::shared_ptr<Expr> Parser::Expression(int precedence) {
  std::shared_ptr<Expr> left = Primary(), right;
  std::shared_ptr<Token> curr_op = Peek();

  if (IsStopToken(curr_op->GetType()))
    return left;

  while (precedence < GetPrecedence(curr_op)) {
    switch (curr_op->GetType()) {
      case TokenType::T_ASSIGN: {
        std::shared_ptr<Token> token = Consume(TokenType::T_ASSIGN);
        right = Expression(GetPrecedence(curr_op) - 1);
        left = std::make_shared<Assign>(std::move(token), left, right);
        break;
      }
      default: {
        Next();
        right = Expression(GetPrecedence(curr_op));
        left = std::make_shared<Binary>(curr_op, left, right);
      }
    }

    curr_op = Peek();
    if (IsStopToken(curr_op->GetType())) {
      return left;
    }
  }

  return left;
}

std::shared_ptr<Expr> Parser::OptionalExpression(int precedence) {
  if (Match(TokenType::T_ASSIGN) || (!Check(TokenType::T_SEMICOLON) && !Check(TokenType::T_COMMA) && !Check(TokenType::T_RPAREN))) {
    return Expression(precedence);
  } else {
    return nullptr;
  }
}

//      }

std::shared_ptr<Expr> Parser::Primary() {
  switch (Peek()->GetType()) {
    case TokenType::T_IDENTIFIER: {
      std::shared_ptr<Literal> literal = std::make_shared<Literal>(Peek());
      literal->is_lvalue = true;
      Next();
      return literal;
    }
    case TokenType::T_INT_LITERAL: {
      std::shared_ptr<Literal> literal = std::make_shared<Literal>(Peek());
      Next();
      return literal;
    }
    default:
      throw ParseException("Expected identifier or literal", Peek());
  }
}

bool Parser::IsStopToken(TokenType type) {
  return type == TokenType::T_SEMICOLON
      || type == TokenType::T_RPAREN
      || type == TokenType::T_COMMA
      || type == TokenType::T_EOF
      || type == TokenType::T_RBRACE;
}

void Parser::synchronize() {
  std::shared_ptr<Token> prev = std::move(Peek());
  Next();
  while (!Check(TokenType::T_EOF)) {
    if (prev->GetType() == TokenType::T_SEMICOLON) return;

    switch (Peek()->GetType()) {
      case TokenType::T_INT:
      case TokenType::T_SHORT:
      case TokenType::T_VOID:
      case TokenType::T_LONG:
      case TokenType::T_IF:
      case TokenType::T_FOR:
      case TokenType::T_WHILE:
      case TokenType::T_DO:
      case TokenType::T_PRINT:
        return;
    }

    Next();
  }
}

std::shared_ptr<Token> Parser::Next() {
  token_ = scanner_.GetToken();
  return token_;
}

bool Parser::Check(TokenType type) {
  return Peek()->GetType() == type;
}

bool Parser::MatchType() {
  return Match(TokenType::T_INT)
    || Match(TokenType::T_SHORT)
    || Match(TokenType::T_LONG)
    || Match(TokenType::T_VOID);
}

bool Parser::Match(TokenType type) {
  if (token_->GetType() == type) {
    Next();
    return true;
  } else {
    return false;
  }
}

std::shared_ptr<Token> Parser::Peek() {
  return token_;
}

std::shared_ptr<Token> Parser::Consume(TokenType type) {
  if (Check(type)) {
    std::shared_ptr<Token> token = Peek();
    Next();
    return std::move(token);
  } else {
    throw ParseException("", Peek());
  }
}

std::shared_ptr<Token> Parser::Consume(TokenType type, const std::string& message) {
  if (Check(type)) {
    std::shared_ptr<Token> token = Peek();
    Next();
    return std::move(token);
  } else {
    throw ParseException(message, Peek());
  }
}

} // namespace mcc
