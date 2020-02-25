//
// Created by vladyslav on 15.01.20.
//

#include "Scanner.h"

namespace mcc {

Scanner::Scanner(FILE* f, ErrorReporter& reporter)
  : reporter_(reporter) {
  source_stream_ = f;
  line_ = 1;
  c_ = 0;
  putback_ = '\0';

  keywords_["print"] = TokenType::T_PRINT;
  keywords_["if"] = TokenType::T_IF;
  keywords_["else"] = TokenType::T_ELSE;
  keywords_["while"] = TokenType::T_WHILE;
  keywords_["for"] = TokenType::T_FOR;
  keywords_["do"] = TokenType::T_DO;
  keywords_["break"] = TokenType::T_BREAK;
  keywords_["continue"] = TokenType::T_CONTINUE;
  keywords_["return"] = TokenType::T_RETURN;
  keywords_["goto"] = TokenType::T_GOTO;
  keywords_["struct"] = TokenType::T_STRUCT;
  keywords_["union"] = TokenType::T_UNION;
  keywords_["enum"] = TokenType::T_ENUM;
  keywords_["typedef"] = TokenType::T_TYPEDEF;
  keywords_["switch"] = TokenType::T_SWITCH;
  keywords_["case"] = TokenType::T_CASE;
  keywords_["default"] = TokenType::T_DEFAULT;
  keywords_["sizeof"] = TokenType::T_SIZEOF;
  keywords_["extern"] = TokenType::T_EXTERN;
  keywords_["static"] = TokenType::T_STATIC;

  // types
  keywords_["void"] = TokenType::T_VOID;
  keywords_["char"] = TokenType::T_CHAR;
  keywords_["short"] = TokenType::T_SHORT;
  keywords_["int"] = TokenType::T_INT;
  keywords_["long"] = TokenType::T_LONG;
}

Scanner::~Scanner() {
  fclose(source_stream_);
}

std::shared_ptr<Token> Scanner::GetToken() {
  char c = NextCharSkipSpaces();
  std::shared_ptr<Token> token = std::make_shared<Token>();
  token->SetLine(line_);
  token->SetCount(c_);
  switch (c) {
    case '+':
      if ((c = Next()) == '+') {
        token->SetType(TokenType::T_INC);
      } else if (c == '=') {
        token->SetType(TokenType::T_ASSIGN_PLUS);
      } else {
        token->SetType(TokenType::T_PLUS);
        Putback(c);
      }
      break;
    case '-':
      if ((c = Next()) == '-') {
        token->SetType(TokenType::T_DEC);
      } else if (c == '>') {
        token->SetType(TokenType::T_ARROW);
      } else if (c == '=') {
        token->SetType(TokenType::T_ASSIGN_MINUS);
      } else {
        token->SetType(TokenType::T_MINUS);
        Putback(c);
      }
      break;
    case '*':
      if ((c = Next()) == '=') {
        token->SetType(TokenType::T_ASSIGN_MUL);
      } else {
        token->SetType(TokenType::T_STAR);
        Putback(c);
      }
      break;
    case '/':
      if ((c = Next()) == '=') {
        token->SetType(TokenType::T_ASSIGN_DIV);
      } else {
        token->SetType(TokenType::T_SLASH);
        Putback(c);
      }
      break;
    case '%':
      if ((c = Next()) == '=') {
        token->SetType(TokenType::T_ASSIGN_MOD);
      } else {
        token->SetType(TokenType::T_MOD);
        Putback(c);
      }
      break;
    case ',':
      token->SetType(TokenType::T_COMMA);
      break;
    case '.':
      token->SetType(TokenType::T_DOT);
      break;
    case ':':
      token->SetType(TokenType::T_COLON);
      break;
    case '?':
      token->SetType(TokenType::T_QUESTION);
      break;
    case '[':
      token->SetType(TokenType::T_LBRACKET);
      break;
    case ']':
      token->SetType(TokenType::T_RBRACKET);
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
    case '\'':
      token->SetInt(ScanChar());
      token->SetType(TokenType::T_INT_LIT);
      if ((c = Next()) != '\'') {
        reporter_.Report("Char literal should end in quote", c, line_, c_);
        Putback(c);
      }
      break;
    case '"':token->SetString(ScanStr());
      token->SetType(TokenType::T_STR_LIT);
      break;
    case '&':
      if ((c = Next()) == '&') {
        token->SetType(TokenType::T_AND);
      } else if (c == '=') {
        token->SetType(TokenType::T_ASSIGN_AND);
      } else {
        token->SetType(TokenType::T_BIT_AND);
        Putback(c);
      }
      break;
    case '|':
      if ((c = Next()) == '|') {
        token->SetType(TokenType::T_OR);
      } else if (c == '=') {
        token->SetType(TokenType::T_ASSIGN_OR);
      } else {
        token->SetType(TokenType::T_BIT_OR);
        Putback(c);
      }
      break;
    case '^':
      if ((c = Next()) == '=') {
        token->SetType(TokenType::T_ASSIGN_XOR);
      } else {
        token->SetType(TokenType::T_BIT_XOR);
        Putback(c);
      }
      break;
    case '>':
      if ((c = Next()) == '=') {
        token->SetType(TokenType::T_GREATER_EQUAL);
      } else if (c == '>') {
        if ((c = Next()) == '=') {
          token->SetType(TokenType::T_ASSIGN_RSHIFT);
        } else {
          token->SetType(TokenType::T_RSHIFT);
          Putback(c);
        }
      } else {
        token->SetType(TokenType::T_GREATER);
        Putback(c);
      }
      break;
    case '<':
      if ((c = Next()) == '=') {
        token->SetType(TokenType::T_LESS_EQUAL);
      } else if (c == '<') {
        if ((c = Next()) == '=') {
          token->SetType(TokenType::T_ASSIGN_LSHIFT);
        } else {
          token->SetType(TokenType::T_LSHIFT);
          Putback(c);
        }
      } else {
        token->SetType(TokenType::T_LESS);
        Putback(c);
      }
      break;
    case '~':
      token->SetType(TokenType::T_NEG);
      break;
    case '!':
      if ((c = Next()) == '=') {
        token->SetType(TokenType::T_NOT_EQUALS);
      } else {
        token->SetType(TokenType::T_NOT);
        Putback(c);
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
        token->SetInt(value);
        token->SetType(TokenType::T_INT_LIT);
      } else if (std::isalpha(c) || c == '_') {
        std::string identifier = ScanIdent(c);
        if (IsKeyword(identifier)) {
          token->SetType(GetKeywordToken(identifier));
        } else {
          token->SetType(TokenType::T_IDENTIFIER);
          token->SetString(identifier);
        }
      } else {
        reporter_.Report("Unrecognizable token", c, line_, c_);
      }
  }

  return token;
}

std::string Scanner::ScanStr() {
  std::string s;
  char c;
  while ((c = (char) ScanChar()) != '"')
    s.push_back(c);
  return std::move(s);
}

int Scanner::ScanChar() {
  char c = Next();

  if (c == '\\') {
    switch (c = Next()) {
      case 'a':  return '\a';
      case 'b':  return '\b';
      case 'f':  return '\f';
      case 'n':  return '\n';
      case 'r':  return '\r';
      case 't':  return '\t';
      case 'v':  return '\v';
      case '\\': return '\\';
      case '"':  return '"' ;
      case '\'': return '\'';
      default:
        reporter_.Report("Unknown escape sequence", c, line_, c_);
        Putback(c);
    }
  }

  return c;
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
    c_--;
  } else {
    c = (char) fgetc(source_stream_);
    if (c == EOF) { // eof
      return EOF;
    }

    while (c == '#') { // preprocessor lines
      std::shared_ptr<Token> token = GetToken();
      if (token->GetType() != TokenType::T_INT_LIT)
        reporter_.Report("Invalid preprocessor output", c, line_, c_);

      int line = token->Int();

      token = GetToken();
      if (token->GetType() != TokenType::T_STR_LIT)
        reporter_.Report("Invalid preprocessor output", c, line_, c_);

      if (token->String()[0] != '<') {
        reporter_.SetFile(token->String());
        line_ = line;
      }

      while ((c = (char) fgetc(source_stream_)) != '\n');
      c = (char) fgetc(source_stream_);
    }
  }

  c_++;
  if (c == '\n') {
    line_++;
    c_ = 0;
  }

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
    if (putback_ == '\n') {
      line_--;
    }
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

