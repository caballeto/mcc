//
// Created by vladyslav on 04.02.20.
//

#ifndef MCC_SRC_TYPE_H_
#define MCC_SRC_TYPE_H_

#include "TokenType.h"

namespace mcc {

enum {
  S_NONE,
  S_STATIC,
  S_EXTERN
};

class Type {
 public:
  Type()
    : name(nullptr), type_(TokenType::T_NONE), ind(0), len(0), storage(S_NONE)
  { }

  bool IsArray() const;
  bool IsPointer() const;
  bool IsStruct() const;
  bool IsUnion() const;
  bool IsPrimitive() const;
  bool IsVoid() const;

  Type& operator=(const Type& type);

  std::shared_ptr<Token> name; // name of type (typedef, union, struct)
  TokenType type_; // type class (char, short, int, long, struct, union) or identifier for typedef
  int ind; // indirection level (number of pointers)
  int len; // length for arrays
  int storage; // (extern, static)
};

std::ostream& operator<<(std::ostream& os, const Type& type);

} // namespace mcc

#endif //MCC_SRC_TYPE_H_
