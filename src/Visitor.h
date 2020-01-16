//
// Created by vladyslav on 16.01.20.
//

#ifndef MCC_SRC_VISITOR_H_
#define MCC_SRC_VISITOR_H_

#include "ast.h"

namespace mcc {

class Visitor {
 public:
  virtual int Visit(const std::shared_ptr<Binary>& binary) = 0;
  virtual int Visit(const std::shared_ptr<Literal>& literal) = 0;
  virtual int Visit(const std::shared_ptr<Print>& print) = 0;
};

} // namespace mcc

#endif //MCC_SRC_VISITOR_H_
