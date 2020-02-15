//
// Created by vladyslav on 04.02.20.
//

#ifndef MCC_SRC_TYPE_CC_
#define MCC_SRC_TYPE_CC_

#include "common.h"
#include "Type.h"
#include "TokenType.h"

namespace mcc {

bool Type::IsArray() const {
  return len > 0;
}

bool Type::IsPointer() const {
  return ind > 0;
}

bool Type::IsStruct() const {
  return type_ == TokenType::T_STRUCT;
}

bool Type::IsPrimitive() const {
  return !IsArray() && !IsPointer() && !IsStruct() && !IsUnion(); // #TODO: add support for typedef
}

bool Type::IsUnion() const {
  return type_ == TokenType::T_UNION;
}

bool Type::IsVoid() const {
  return type_ == TokenType::T_VOID;
}

Type& Type::operator=(const Type& type) {
  if (&type == this) return *this;
  type_ = type.type_;
  ind = type.ind;
  len = type.len;
  return *this;
}

std::ostream& operator<<(std::ostream& os, const Type& type) {
  os << "'" << type.type_;
  for (int i = 0; i < type.ind; i++)
    os << '*';
  os << "'";
  return os;
}

} // namespace mcc

#endif //MCC_SRC_TYPE_CC_

