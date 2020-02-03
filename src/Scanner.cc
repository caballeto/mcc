//
// Created by vladyslav on 15.01.20.
//

#include "Scanner.h"

namespace mcc {

Scanner::Scanner(const std::string& input_file) {
  source_stream_.open(input_file, std::ios::in);
  source_stream_ >> std::noskipws;
  line_ = 1;
  putback_ = '\0';

  keywords_["print"] = TokenType::T_PRINT;
  keywords_["int"] = TokenType::T_INT;
  keywords_["if"] = TokenType::T_IF;
  keywords_["else"] = TokenType::T_ELSE;
  keywords_["while"] = TokenType::T_WHILE;
  keywords_["for"] = TokenType::T_FOR;
  keywords_["do"] = TokenType::T_DO;
  keywords_["break"] = TokenType::T_BREAK;
  keywords_["continue"] = TokenType::T_CONTINUE;
}

Scanner::~Scanner() {
  source_stream_.close();
}

std::shared_ptr<Token> Scanner::GetToken() {
  char c = NextCharSkipSpaces();
  std::shared_ptr<Token> token = std::make_shared<Token>();
  token->SetLine(line_);
  switch (c) {
    case '+':
      token->SetType(TokenType::T_PLUS);
      break;
    case '-':
      token->SetType(TokenType::T_MINUS);
      break;
    case '*':
      token->SetType(TokenType::T_STAR);
      break;
    case '/':
      token->SetType(TokenType::T_SLASH);
      break;
    case ',':
      token->SetType(TokenType::T_COMMA);
      break;
    case ';':
      token->SetType(TokenType::T_SEMICOLON);
      break;
    case '(':
      token->SetType(TokenType::T_LPAREN);
      break;
    case ')':
      token->SetType(TokenType::T_RPAREN);
      break;
    case '{':
      token->SetType(TokenType::T_LBRACE);
      break;
    case '}':
      token->SetType(TokenType::T_RBRACE);
      break;
    case '>':
      if ((c = Next()) == '=') {
        token->SetType(TokenType::T_GREATER_EQUAL);
      } else {
        token->SetType(TokenType::T_GREATER);
        Putback(c);
      }
      break;
    case '<':
      if ((c = Next()) == '=') {
        token->SetType(TokenType::T_LESS_EQUAL);
      } else {
        token->SetType(TokenType::T_LESS);
        Putback(c);
      }
      break;
    case '!':
      if ((c = Next()) == '=') {
        token->SetType(TokenType::T_NOT_EQUALS);
      } else {
        Putback(c); // #TODO: Add T_NOT bool operator
        std::cerr << "Only '!=' operator supported now." << std::endl;
        exit(1);
      }
      break;
    case '=':
      if ((c = Next()) == '=') {
        token->SetType(TokenType::T_EQUALS);
      } else {
        token->SetType(TokenType::T_ASSIGN);
        Putback(c);
      }
      break;
    case EOF:
      token->SetType(TokenType::T_EOF);
      break;
    default:
      if (std::isdigit(c)) {
        int value = ScanInt(c - '0');
        //std::cout << "Parsed Int: " << value << std::endl;
        token->SetIntValue(value);
        token->SetType(TokenType::T_INT_LITERAL);
      } else if (std::isalpha(c) || c == '_') {
        std::string identifier = ScanIdent(c);
        if (IsKeyword(identifier)) {
          token->SetType(GetKeywordToken(identifier));
        } else {
          token->SetType(TokenType::T_IDENTIFIER);
          token->SetStringValue(identifier);
        }
      } else {
        std::cerr << "Char: '" << c << "'" << std::endl;
        std::cerr << "Can't recognize token." << std::endl;
        exit(1);
      }
  }

  return token;
}

char Scanner::NextCharSkipSpaces() {
  char c = Next();
  while (c == ' ' || c == '\t' || c == '\r' || c == '\n')
    c = Next();
  return c;
}

char Scanner::Next() {
  char c;

  if (putback_ != '\0') {
    c = putback_;
    putback_ = '\0';
  } else {
    if (!(source_stream_ >> c)) {
      return EOF;
    }
  }

  if (c == '\n')
    line_++;
  return c;
}

int Scanner::ScanInt(int int_value) {
  char c = Next();
  while (c != EOF && std::isdigit(c)) {
    int_value = int_value * 10 + (c - '0');
    c = Next();
  }

  Putback(c);
  return int_value;
}

std::string Scanner::ScanIdent(char c) {
  std::string identifier;
  identifier += c;
  c = Next();
  while (c != EOF && (std::isalnum(c) || c == '_')) {
    identifier += c;
    c = Next();
  }

  Putback(c);
  return std::move(identifier); // copy elision, even without move
}

void Scanner::Putback(char c) {
  if (putback_ == '\0') {
    putback_ = c;
  } else {
    std::cerr << "Putback is already filled." << std::endl;
    exit(1);
  }
}

bool Scanner::IsKeyword(const std::string& identifier) {
  return keywords_.count(identifier) != 0;
}

// Function assumes that passed reference is a keywords
TokenType Scanner::GetKeywordToken(const std::string& keyword) {
  return keywords_[keyword];
}

} // namespace mcc

