//
// Created by vladyslav on 16.01.20.
//

#ifndef MCC_SRC_SYMBOLTABLE_H_
#define MCC_SRC_SYMBOLTABLE_H_

#include "common.h"
#include "Type.h"

#define ENTRY_LIMIT 1024

namespace mcc {

struct Entry {
  std::string name;
  Type type;
  int indirection;
};

// #FIXME: redesign a better (faster, simpler) symbol table
class SymbolTable {
 public:

  Entry& Get(int id);

  int Get(const std::string& name);

  int Put(const std::string& name, Type type, int indirection);

  bool Contains(const std::string& name);

  bool Remove(const std::string& name);

 private:
  int entry_count_ = 0;
  Entry entries_[ENTRY_LIMIT];
};

} // namespace mcc

#endif //MCC_SRC_SYMBOLTABLE_H_
