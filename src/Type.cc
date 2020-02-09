//
// Created by vladyslav on 04.02.20.
//

#ifndef MCC_SRC_TYPE_CC_
#define MCC_SRC_TYPE_CC_

#include "common.h"
#include "Type.h"
#include "TokenType.h"

namespace mcc {

Type TokenToType(TokenType type) {
  switch (type) {
    case TokenType::T_VOID: return Type::VOID;
    case TokenType::T_SHORT: return Type::SHORT;
    case TokenType::T_INT: return Type::INT;
    case TokenType::T_LONG: return Type::LONG;
    default: {
      std::cerr << "InternalError: Invalid TokenType in TokenToType() " << type << std::endl;
      exit(1);
    }
  }
}

std::ostream& operator<<(std::ostream& os, Type type) {
  switch (type) {
    case Type::VOID: return os << "void";
    case Type::SHORT: return os << "short";
    case Type::INT: return os << "int";
    case Type::LONG: return os << "long";
    case Type::ARRAY: return os << "array []";
    case Type::NONE: return os << "NO TYPE";
  }
  return os << "InternalError: uncovered type in switch" << type << std::endl;
}

} // namespace mcc

#endif //MCC_SRC_TYPE_CC_

