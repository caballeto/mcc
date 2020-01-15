//
// Created by vladyslav on 15.01.20.
//

#include "ast.h"

namespace mcc {

// #TODO: change std::endl to \n for better bufferization?

void Binary::Dump(std::ostream& os, int spaces) {
  os << std::string(spaces, ' ') << "<binary op='" << op_->GetType() << "'>" << std::endl;
  left_->Dump(os, spaces + 2);
  right_->Dump(os, spaces + 2);
  os << std::string(spaces, ' ') << "</binary>" << std::endl;
}

int Binary::Accept(Visitor& visitor) {
  return visitor.Visit(std::static_pointer_cast<Binary>(shared_from_this()));
}

void Literal::Dump(std::ostream& os, int spaces) {
  os << std::string(spaces, ' ') << "<literal val='" << std::to_string(literal_->GetIntValue())
     << "'></literal>" << std::endl;
}

int Literal::Accept(Visitor& visitor) {
  return visitor.Visit(std::static_pointer_cast<Literal>(shared_from_this()));
}

} // namespace mcc