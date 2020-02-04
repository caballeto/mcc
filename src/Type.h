//
// Created by vladyslav on 04.02.20.
//

#ifndef MCC_SRC_TYPE_H_
#define MCC_SRC_TYPE_H_

#include "TokenType.h"

namespace mcc {

enum class Type {
  VOID = 0,
  SHORT = 1,
  INT = 2,
  LONG = 3,
  NONE
};

std::ostream& operator<<(std::ostream& os, Type type);
Type TokenToType(TokenType type);

} // namespace mcc

#endif //MCC_SRC_TYPE_H_
