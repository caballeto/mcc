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
      stmts.push_back(Declaration());
    } catch (const ParseException& e) {
      reporter_.ReportParseError(e);
      SynchronizeDeclaration();
    }
  }

  return std::move(stmts);
}

std::shared_ptr<Stmt> Parser::GlobalVarDeclaration(Type type, std::shared_ptr<Token> name) {
  std::vector<std::shared_ptr<VarDecl>> var_decl_list;
  std::shared_ptr<Token> token = name;
  std::shared_ptr<Expr> init;

  if (Match(TokenType::T_LBRACKET)) {
    std::shared_ptr<Token> len = Consume(TokenType::T_INT_LIT, "Expected integer length > 0");
    type.len = len->Int();
    Consume(TokenType::T_RBRACKET, "Expected ']' after array length declaration");
    var_decl_list.push_back(std::make_shared<VarDecl>(name, nullptr, type, false, false));
    type.len = 0;
  } else {
    init = OptionalExpression(0);
    var_decl_list.push_back(std::make_shared<VarDecl>(name, init, type, true, false));
  }

  if (!Check(TokenType::T_SEMICOLON)) {
    do {
      int ind = 0;
      while (Match(TokenType::T_STAR)) ind++;
      type.ind = ind;
      name = Consume(TokenType::T_IDENTIFIER, "Expected identifier after type declaration");
      if (Match(TokenType::T_LBRACKET)) {
        std::shared_ptr<Token> len = Consume(TokenType::T_INT_LIT, "Expected integer length > 0");
        type.len = len->Int();
        Consume(TokenType::T_RBRACKET, "Expected ']' after array length declaration");
        var_decl_list.push_back(std::make_shared<VarDecl>(name, nullptr, type, false, false));
        type.len = 0;
      } else {
        init = OptionalExpression(0);
        var_decl_list.push_back(std::make_shared<VarDecl>(name, init, type, true, false));
      }
    } while (Match(TokenType::T_COMMA));
  }

  Consume(TokenType::T_SEMICOLON, "Expected ';' after global variable declaration");
  return std::make_shared<DeclList>(std::move(token), std::move(var_decl_list));
}

// #FIXME: for strucrs/unions
std::shared_ptr<Stmt> Parser::Declaration() {
  Type type = ParsePrim();

  if (Check(TokenType::T_LBRACE)) { // struct declaration
    if (type.type_ == TokenType::T_STRUCT)
      return StructDeclaration(type);
    return UnionDeclaration(type);
  }

  int indirection = 0;
  while (Match(TokenType::T_STAR)) indirection++;
  type.ind = indirection;
  std::shared_ptr<Token> name = Consume(TokenType::T_IDENTIFIER, "Identifier expected in global declaration");

  if (Match(TokenType::T_LPAREN)) { // function
    std::shared_ptr<DeclList> signature = ParameterList(TokenType::T_COMMA, TokenType::T_RPAREN);
    Consume(TokenType::T_RPAREN, "')' expected after function declaration");
    std::shared_ptr<Block> block = nullptr;
    if (Check(TokenType::T_LBRACE)) {
      block = BlockStatement();
    } else {
      Consume(TokenType::T_SEMICOLON, "';' expected after function prototype");
    }

    return std::make_shared<FuncDecl>(std::move(type), name, signature, block);
  } else { // global var declaration
    return GlobalVarDeclaration(type, name);
  }
}

std::shared_ptr<Union> Parser::UnionDeclaration(const Type &type) {
  std::shared_ptr<Token> token = Consume(TokenType::T_LBRACE);
  std::shared_ptr<DeclList> decl_list = ParameterList(TokenType::T_SEMICOLON, TokenType::T_RBRACE);
  Consume(TokenType::T_RBRACE, "'}' expected after union declaration");
  std::shared_ptr<Token> var_name = Check(TokenType::T_IDENTIFIER) ? Consume(TokenType::T_IDENTIFIER) : nullptr;
  Consume(TokenType::T_SEMICOLON, "';' expected after union declaration");
  return std::make_shared<Union>(token, type, decl_list, std::move(var_name));
}

std::shared_ptr<Struct> Parser::StructDeclaration(const Type& type) {
  std::shared_ptr<Token> token = Consume(TokenType::T_LBRACE);
  std::shared_ptr<DeclList> decl_list = ParameterList(TokenType::T_SEMICOLON, TokenType::T_RBRACE);
  Consume(TokenType::T_RBRACE, "'}' expected after struct declaration");
  std::shared_ptr<Token> var_name = Check(TokenType::T_IDENTIFIER) ? Consume(TokenType::T_IDENTIFIER) : nullptr;
  Consume(TokenType::T_SEMICOLON, "';' expected after struct declaration");
  return std::make_shared<Struct>(token, type, decl_list, std::move(var_name));
}

// #FIXME: global elements could only have CONSTANT inits (add type check + fix parser)
// #FIXME: local variables could use themselves in their own inits (will be written to some mem (e. g. addq $1, -4(%rbp))
std::shared_ptr<Stmt> Parser::Statement() {
  switch (Peek()->GetType()) {
    case TokenType::T_PRINT:    return PrintStatement();
    case TokenType::T_UNION:
    case TokenType::T_STRUCT:
    case TokenType::T_VOID:
    case TokenType::T_CHAR:
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
    case TokenType::T_RETURN:   return ReturnStatement();
    case TokenType::T_GOTO:     return GotoStatement();
    default:
      if (Check(TokenType::T_IDENTIFIER) && PeekNext()->GetType() == TokenType::T_COLON) {
        std::shared_ptr<Token> name = Consume(TokenType::T_IDENTIFIER);
        Consume(TokenType::T_COLON, "':' expected after label");
        return std::make_shared<Label>(name);
      }
      return ExpressionStatement();
  }
}

std::shared_ptr<Return> Parser::ReturnStatement() {
  std::shared_ptr<Token> token = Consume(TokenType::T_RETURN);
  std::shared_ptr<Expr> expr = Expression(0);
  Consume(TokenType::T_SEMICOLON, "Expected ';' after return statement");
  return std::make_shared<Return>(token, expr);
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
  if (CheckType()) {
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

std::shared_ptr<DeclList> Parser::ParameterList(TokenType delim, TokenType stop) {
  std::shared_ptr<Token> token = Peek();
  std::vector<std::shared_ptr<VarDecl>> var_decl_list;

  do {
    if (Check(stop)) break;
    Type type = ParsePrim();
    int indirection = 0;
    while (Match(TokenType::T_STAR)) indirection++;
    type.ind = indirection;
    std::shared_ptr<Token> name = Consume(TokenType::T_IDENTIFIER,"Expected identifier after type declaration");
    if (Match(TokenType::T_LBRACKET)) {
      std::shared_ptr<Token> len = Consume(TokenType::T_INT_LIT, "Expected integer length > 0");
      Consume(TokenType::T_RBRACKET, "Expected ']' after array length declaration");
      type.len = len->Int();
    }
    var_decl_list.push_back(std::make_shared<VarDecl>(name, nullptr, std::move(type), false, true));
  } while (Match(delim));

  return std::make_shared<DeclList>(token, var_decl_list);
}

std::shared_ptr<DeclList> Parser::DeclarationList() {
  std::vector<std::shared_ptr<VarDecl>> var_decl_list;
  std::shared_ptr<Token> token = Peek();
  Type type = ParsePrim();

  do {
    int ind = 0;
    while (Match(TokenType::T_STAR)) ind++;
    type.ind = ind;
    std::shared_ptr<Token> name = Consume(TokenType::T_IDENTIFIER,"Expected identifier after type declaration");
    if (Match(TokenType::T_LBRACKET)) {
      std::shared_ptr<Token> len = Consume(TokenType::T_INT_LIT, "Expected integer length > 0");
      Consume(TokenType::T_RBRACKET, "Expected ']' after array length declaration");
      type.len = len->Int();
      var_decl_list.push_back(std::make_shared<VarDecl>(name, nullptr, type, false, true));
      type.len = 0;
    } else {
      std::shared_ptr<Expr> init = OptionalExpression(0);
      var_decl_list.push_back(std::make_shared<VarDecl>(name, init, type, false, true));
    }
  } while (Match(TokenType::T_COMMA));

  Consume(TokenType::T_SEMICOLON, "Expected ';' after variable declaration");
  return std::make_shared<DeclList>(std::move(token), var_decl_list);
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
  std::shared_ptr<Expr> expr = Expression(0);
  Consume(TokenType::T_SEMICOLON, "Expected ';' after expression statement");
  return std::make_shared<ExpressionStmt>(nullptr, expr);
}

std::shared_ptr<Block> Parser::BlockStatement() {
  std::shared_ptr<Token> token = Consume(TokenType::T_LBRACE, "Expected '{' at start of block");
  std::vector<std::shared_ptr<Stmt>> stmts;

  while (!Check(TokenType::T_RBRACE) && !Check(TokenType::T_EOF)) {
    try {
      stmts.push_back(Statement());
    } catch (const ParseException& e) {
      reporter_.ReportParseError(e);
      Synchronize();
    }
  }

  Consume(TokenType::T_RBRACE, "Expected '}' after block");
  return std::make_shared<Block>(token, std::move(stmts));
}

std::shared_ptr<GoTo> Parser::GotoStatement() {
  Match(TokenType::T_GOTO);
  std::shared_ptr<Token> name = Consume(TokenType::T_IDENTIFIER, "Label name expected");
  Consume(TokenType::T_SEMICOLON, "';' expected after goto statement");
  return std::make_shared<GoTo>(std::move(name));
}

// Read about named local return value optimization and copy-elision
std::shared_ptr<Expr> Parser::Expression(int precedence) {
  std::shared_ptr<Expr> left = Primary(), right;
  std::shared_ptr<Token> curr_op = Peek();

  if (IsStopToken(curr_op->GetType()))
    return left;

  while (precedence < GetPrecedence(curr_op, false)) {
    switch (curr_op->GetType()) {
      case TokenType::T_LPAREN: {
        Next();
        std::shared_ptr<ExprList> args = ExpressionList();
        Consume(TokenType::T_RPAREN, "')' expected after call");
        left = std::make_shared<Call>(curr_op, left, args);
        break;
      }
      case TokenType::T_LBRACKET: {
        Next();
        std::shared_ptr<Expr> index = Expression(0);
        Consume(TokenType::T_RBRACKET, "']' expected after array indexing operation");
        left = std::make_shared<Index>(curr_op, left, index);
        break;
      }
      case TokenType::T_INC:
      case TokenType::T_DEC: {
        Next();
        left = std::make_shared<Postfix>(curr_op, left);
        break;
      }
      case TokenType::T_QUESTION: {
        Next();
        std::shared_ptr<Expr> then_branch = Expression(0);
        Consume(TokenType::T_COLON, "':' expected after '?'");
        std::shared_ptr<Expr> else_branch = Expression(GetPrecedence(curr_op, false) - 1);
        left = std::make_shared<Ternary>(curr_op, left, then_branch, else_branch);
        break;
      }
      case TokenType::T_ASSIGN: {
        Next();
        right = Expression(GetPrecedence(curr_op, false) - 1);
        left = std::make_shared<Assign>(std::move(curr_op), left, right);
        break;
      }
      case TokenType::T_ARROW:
      case TokenType::T_DOT: {
        Next();
        right = Expression(GetPrecedence(curr_op, false));
        left = std::make_shared<Access>(std::move(curr_op), left, right);
        break;
      }
      default: {
        Next();
        right = Expression(GetPrecedence(curr_op, false));
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
  if (Match(TokenType::T_ASSIGN) || (
      !Check(TokenType::T_SEMICOLON) &&
      !Check(TokenType::T_COMMA) &&
      !Check(TokenType::T_RPAREN) &&
      !Check(TokenType::T_LBRACKET))) {
    return Expression(precedence);
  } else {
    return nullptr;
  }
}

std::shared_ptr<Expr> Parser::Primary() {
  switch (Peek()->GetType()) {
    case TokenType::T_IDENTIFIER: {
      std::shared_ptr<Literal> literal = std::make_shared<Literal>(Peek());
      literal->is_lvalue_ = true;
      Next();
      return literal;
    }
    case TokenType::T_STR_LIT:
    case TokenType::T_INT_LIT: {
      std::shared_ptr<Literal> literal = std::make_shared<Literal>(Peek());
      Next();
      return literal;
    }
    case TokenType::T_LPAREN: {
      std::shared_ptr<Token> paren = Peek();
      Next();
      std::shared_ptr<Expr> expr = Expression(0);
      Consume(TokenType::T_RPAREN, "')' expected after grouping expression");
      return std::make_shared<Grouping>(paren, expr);
    }
    case TokenType::T_INC:
    case TokenType::T_DEC:
    case TokenType::T_NEG:
    case TokenType::T_NOT:
    case TokenType::T_PLUS:
    case TokenType::T_MINUS:
    case TokenType::T_BIT_AND:
    case TokenType::T_STAR: {
      std::shared_ptr<Token> op = Peek();
      Next();
      std::shared_ptr<Expr> right = Expression(GetPrecedence(op, true) - 1);
      return std::make_shared<Unary>(op, right);
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
      || type == TokenType::T_RBRACE
      || type == TokenType::T_COLON
      || type == TokenType::T_RBRACKET;
}

bool Parser::CheckType() {
  switch (Peek()->GetType()) {
    case TokenType::T_CHAR:
    case TokenType::T_SHORT:
    case TokenType::T_INT:
    case TokenType::T_LONG:
    case TokenType::T_VOID:
    case TokenType::T_STRUCT:
    case TokenType::T_UNION:
      return true;
  }
  return false;
}

Type Parser::ParsePrim() {
  Type type;

  switch (Peek()->GetType()) {
    case TokenType::T_CHAR:
    case TokenType::T_SHORT:
    case TokenType::T_INT:
    case TokenType::T_LONG:
    case TokenType::T_VOID:
      type.type_ = Peek()->GetType();
      Next();
      break;
    case TokenType::T_UNION:
    case TokenType::T_STRUCT:
      type.type_ = Peek()->GetType();
      Next();
      if (Check(TokenType::T_IDENTIFIER))
        type.name = Consume(TokenType::T_IDENTIFIER);
      break;
    default: {
      throw ParseException("Could not parse type", Peek());
    }
  }

  return std::move(type);
}

void Parser::Synchronize() {
  if (Peek()->GetType() == TokenType::T_SEMICOLON) {
    Next();
    return;
  }

  while (!Check(TokenType::T_EOF)) {
    switch (Peek()->GetType()) {
      case TokenType::T_INT:
      case TokenType::T_CHAR:
      case TokenType::T_SHORT:
      case TokenType::T_VOID:
      case TokenType::T_LONG:
      case TokenType::T_IF:
      case TokenType::T_FOR:
      case TokenType::T_WHILE:
      case TokenType::T_DO:
      case TokenType::T_PRINT:
      case TokenType::T_RBRACE:
        return;
    }

    Next();
  }
}

void Parser::SynchronizeDeclaration() {
  while (!Check(TokenType::T_EOF)) {
    switch (Peek()->GetType()) {
      case TokenType::T_INT:
      case TokenType::T_CHAR:
      case TokenType::T_SHORT:
      case TokenType::T_VOID:
      case TokenType::T_LONG:
        return;
    }

    Next();
  }
}

std::shared_ptr<Token> Parser::Next() {
  if (look_ahead_ != nullptr) {
    token_ = look_ahead_;
    look_ahead_ = nullptr;
    return token_;
  }

  token_ = scanner_.GetToken();
  return token_;
}

std::shared_ptr<Token> Parser::PeekNext() {
  if (look_ahead_ != nullptr) return look_ahead_;
  look_ahead_ = scanner_.GetToken();
  return look_ahead_;
}

bool Parser::Check(TokenType type) {
  return Peek()->GetType() == type;
}

bool Parser::MatchType() {
  return Match(TokenType::T_INT)
    || Match(TokenType::T_CHAR)
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
