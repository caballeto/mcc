//
// Created by vladyslav on 15.01.20.
//

#include "Parser.h"

namespace mcc {

std::vector<std::shared_ptr<Stmt>> Parser::Parse() {
  std::vector<std::shared_ptr<Stmt>> stmts;

  while (!Check(TokenType::T_EOF)) {
    stmts.push_back(Statement());
  }

  return std::move(stmts);
}

// #FIXME: global elements could only have CONSTANT inits (add type check + fix parser)
// #FIXME: local variables could use themselves in their own inits (will be written to some mem (e. g. addq $1, -4(%rbp))
// #TODO: decompose into separate functions for readability
std::shared_ptr<Stmt> Parser::Statement() {
  switch (Peek()->GetType()) {
    case TokenType::T_PRINT:
      return PrintStatement();
    case TokenType::T_VOID: {
      reporter_.Report("'void' is not allowed as type", Peek());
      exit(1);
    }
    case TokenType::T_INT:
    case TokenType::T_SHORT:
    case TokenType::T_LONG:
      return DeclarationList();
    case TokenType::T_IF:
      return IfStatement();
    case TokenType::T_WHILE:
      return WhileStatement();
    case TokenType::T_DO:
      return DoWhileStatement();
    case TokenType::T_FOR:
      return ForStatement();
    case TokenType::T_LBRACE:
      return BlockStatement();
    case TokenType::T_BREAK:
      return BreakStatement();
    case TokenType::T_CONTINUE:
      return ContinueStatement();
    default:
      return ExpressionStatement();
  }
}

std::shared_ptr<ControlFlow> Parser::BreakStatement() {
  Match(TokenType::T_BREAK);
  Consume(TokenType::T_SEMICOLON, "Expected ';' after 'break' Statement.");
  return std::make_shared<ControlFlow>(true);
}

std::shared_ptr<ControlFlow> Parser::ContinueStatement() {
  Match(TokenType::T_CONTINUE);
  Consume(TokenType::T_SEMICOLON, "Expected ';' after 'continue' Statement.");
  return std::make_shared<ControlFlow>(false);
}

std::shared_ptr<Print> Parser::PrintStatement() {
  Match(TokenType::T_PRINT);
  std::shared_ptr<Expr> expr = Expression(0);
  Consume(TokenType::T_SEMICOLON, "Expected ';' after 'print' Statement.");
  return std::make_shared<Print>(expr);
}

// #FIXME: move semantic cheks to semantic analyzer
std::shared_ptr<Conditional> Parser::IfStatement() {
  std::shared_ptr<Token> if_token = Consume(TokenType::T_IF);
  Consume(TokenType::T_LPAREN, "Expected '(' after 'if'.");
  std::shared_ptr<Expr> condition = Expression(0);
  Consume(TokenType::T_RPAREN, "Expected ')' after 'if' condition.");
  std::shared_ptr<Stmt> then_block_ = Statement(), else_block_;
  DisallowDecl(then_block_, if_token);

  if (Check(TokenType::T_ELSE)) {
    std::shared_ptr<Token> else_token = Consume(TokenType::T_ELSE);
    else_block_ = Statement();
    DisallowDecl(else_block_, else_token);
  }

  return std::make_shared<Conditional>(condition, then_block_, else_block_);
}

std::shared_ptr<While> Parser::DoWhileStatement() {
  std::shared_ptr<Token> do_token = Consume(TokenType::T_DO);
  std::shared_ptr<Stmt> loop_block = Statement();
  DisallowDecl(loop_block, do_token);
  Consume(TokenType::T_WHILE, "Expected 'while' after 'do'.");
  Consume(TokenType::T_LPAREN, "Expected '(' after 'while'.");
  std::shared_ptr<Expr> condition = Expression(0);
  Consume(TokenType::T_RPAREN, "Expected ')' after 'while' condition.");
  Consume(TokenType::T_SEMICOLON, "Expected closing ';' after do-while statement.");
  return std::make_shared<While>(condition, loop_block, true);
}

// #FIXME: make condition expression optional by adding break/continue
std::shared_ptr<For> Parser::ForStatement() {
  std::shared_ptr<Token> for_token = Consume(TokenType::T_FOR);
  Consume(TokenType::T_LPAREN, "Expected '(' after 'for'.");

  std::shared_ptr<Stmt> init_list;
  if (Check(TokenType::T_INT)) {
    init_list = DeclarationList();
  } else {
    init_list = ExpressionList();
    Consume(TokenType::T_SEMICOLON, "Expected ';' after init in for statement.");
  }

  std::shared_ptr<Expr> condition = OptionalExpression(0);
  Consume(TokenType::T_SEMICOLON, "Expected ';' after condition in for statement.");

  std::shared_ptr<Stmt> update_list = ExpressionList();
  Consume(TokenType::T_RPAREN, "Expected closing ')' after 'for' statement.");

  std::shared_ptr<Stmt> loop_block = Statement();
  DisallowDecl(loop_block, for_token);
  return std::make_shared<For>(init_list, condition, update_list, loop_block);
}

void Parser::DisallowDecl(const std::shared_ptr<Stmt>& stmt, const std::shared_ptr<Token>& token) {
  if (stmt->IsDeclaration()) {
    reporter_.Report("Declaration are not allowed in single-statement loops", token);
    exit(1);
  }
}

std::shared_ptr<While> Parser::WhileStatement() {
  std::shared_ptr<Token> while_token = Consume(TokenType::T_WHILE);
  Consume(TokenType::T_LPAREN, "Expected '(' after 'while'.");
  std::shared_ptr<Expr> condition = Expression(0);
  Consume(TokenType::T_RPAREN, "Expected ')' after 'while' condition.");
  std::shared_ptr<Stmt> loop_block = Statement();
  DisallowDecl(loop_block, while_token);
  return std::make_shared<While>(condition, loop_block, false);
}

std::shared_ptr<DeclList> Parser::DeclarationList() {
  std::shared_ptr<Token> type_token = Peek();
  MatchType();
  std::vector<std::shared_ptr<VarDecl>> var_decl_list;

  do {
    std::shared_ptr<Token> name = Consume(TokenType::T_IDENTIFIER,"Expected identifier after type declaration.");
    int id = symbol_table_.Put(name->GetStringValue(), type_token->GetType(), 0);
    std::shared_ptr<Expr> init = OptionalExpression(0);
    var_decl_list.push_back(std::make_shared<VarDecl>(name->GetStringValue(), init, id));
  } while (Match(TokenType::T_COMMA));

  Consume(TokenType::T_SEMICOLON, "Expected ';' after variable declaration.");
  return std::make_shared<DeclList>(var_decl_list);
}

std::shared_ptr<ExprList> Parser::ExpressionList() {
  std::vector<std::shared_ptr<Expr>> expr_list;

  if (!Check(TokenType::T_SEMICOLON) && !Check(TokenType::T_RPAREN)) {
    do {
      expr_list.push_back(Expression(0));
    } while (Match(TokenType::T_COMMA));
  }

  return std::make_shared<ExprList>(expr_list);
}

std::shared_ptr<Stmt> Parser::ExpressionStatement() {
  switch (Peek()->GetType()) {
    case TokenType::T_IDENTIFIER: {
      std::shared_ptr<Expr> expr = Expression(0);
      Consume(TokenType::T_SEMICOLON, "Expected ';' after expression Statement.");
      return std::make_shared<ExpressionStmt>(expr);
    }
    default:
      std::cerr << "Only assign expressions are supported in top-level expression statements." << std::endl;
      exit(1);
  }
}

// Read about named local return value optimization and copy-elision
std::shared_ptr<Expr> Parser::Expression(int precedence) {
  std::shared_ptr<Expr> left = Primary(), right;
  std::shared_ptr<Token> curr_op = Peek();

  if (curr_op->GetType() == TokenType::T_SEMICOLON
    || curr_op->GetType() == TokenType::T_RPAREN
    || curr_op->GetType() == TokenType::T_COMMA)
    return left;

  while (precedence < GetPrecedence(curr_op->GetType())) {
    switch (curr_op->GetType()) {
      case TokenType::T_ASSIGN: {
        if (!left->IsVariable()) {
          std::cerr << "Only variables are allowed on left side of assign expression." << std::endl;
          exit(1);
        }

        Next();
        right = Expression(GetPrecedence(curr_op->GetType()) - 1);
        left = std::make_shared<Assign>(left, right);
        break;
      }
      default: {
        Next();
        right = Expression(GetPrecedence(curr_op->GetType()));
        left = std::make_shared<Binary>(curr_op, left, right);
      }
    }

    curr_op = Peek();
    if (curr_op->GetType() == TokenType::T_SEMICOLON
      || curr_op->GetType() == TokenType::T_RPAREN
      || curr_op->GetType() == TokenType::T_COMMA) {
      return left;
    }
  }

  return left;
}

std::shared_ptr<Expr> Parser::OptionalExpression(int precedence) {
  if (Match(TokenType::T_ASSIGN)) {
    return Expression(precedence);
  } else {
    return nullptr;
  }
}

std::shared_ptr<Expr> Parser::Primary() {
  switch (Peek()->GetType()) {
    case TokenType::T_IDENTIFIER: {
      std::shared_ptr<Literal> literal = std::make_shared<Literal>(Peek());

      if (!symbol_table_.Contains(literal->literal_->GetStringValue())) {
        std::cerr << "Variable '" << literal->literal_->GetStringValue() << "' has not been declared." << std::endl;
        exit(1);
      }

      Next();
      return literal;
    }
    case TokenType::T_INT_LITERAL: {
      std::shared_ptr<Literal> literal = std::make_shared<Literal>(Peek());
      Next();
      return literal;
    }
    default:
      std::cerr << " Error on line " << Peek()->GetLine() <<
        " Primary expects int or identifier literal." << std::endl;
      exit(1);
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
  return Match(TokenType::T_INT) || Match(TokenType::T_SHORT) || Match(TokenType::T_LONG);
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
    exit(1); // #TODO: add sync mechanism
  }
}

std::shared_ptr<Token> Parser::Consume(TokenType type, const std::string& message) {
  if (Check(type)) {
    std::shared_ptr<Token> token = Peek();
    Next();
    return std::move(token);
  } else {
    reporter_.Report(message, Peek());
    exit(1); // #TODO: add sync mechanism
  }
}

std::shared_ptr<Block> Parser::BlockStatement() {
  Consume(TokenType::T_LBRACE, "Expected '{' at start of block.");
  std::vector<std::shared_ptr<Stmt>> stmts;

  while (!Check(TokenType::T_RBRACE)) {
    stmts.push_back(Statement());
  }

  Match(TokenType::T_RBRACE);

  return std::make_shared<Block>(std::move(stmts));
}

} // namespace mcc
