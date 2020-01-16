//
// Created by vladyslav on 16.01.20.
//

#include "SymbolTable.h"

namespace mcc {

int SymbolTable::Get(const std::string &name) {
  for (int i = 0; i <= entry_count_; i++) {
    if (entries_[i].name == name) {
      return i;
    }
  }
  return -1;
}

int SymbolTable::Put(const std::string& name) {
  entries_[++entry_count_] = {name};
  return entry_count_;
}

bool SymbolTable::Contains(const std::string &name) {
  return Get(name) != -1;
}

bool SymbolTable::Remove(const std::string &name) {
  for (int i = 0; i < entry_count_; i++) {
    if (entries_[i].name == name) {
      entries_[i].name = "";
      return true;
    }
  }
  return false;
}

} // namespace mcc