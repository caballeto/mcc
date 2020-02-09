//
// Created by vladyslav on 16.01.20.
//

#include "SymbolTable.h"

namespace mcc {

Entry& SymbolTable::Get(int id) {
  return entries_[id];
}

int SymbolTable::Get(const std::string &name) {
  for (int i = 0; i <= entry_count_; i++) {
    if (entries_[i].name == name) {
      return i;
    }
  }
  return -1;
}

int SymbolTable::Put(const std::string& name, Type type, int indirection, int len, bool is_function) {
  entries_[++entry_count_] = {name, type, indirection, len, is_function};
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

int SymbolTable::Put(const std::shared_ptr<FuncDecl>& func_decl) {
  return Put(func_decl->name_->GetStringValue(), func_decl->return_type_, func_decl->indirection_, 0, true);
}

} // namespace mcc