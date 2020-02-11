//
// Created by vladyslav on 16.01.20.
//

#include "SymbolTable.h"

namespace mcc {

Entry* SymbolTable::Get(const std::string& name) {
  for (auto i = scopes_.rbegin(); i != scopes_.rend(); ++i) {
    if ((*i).count(name) != 0) {
      return &(*i)[name];
    }
  }

  return nullptr;
}

Entry* SymbolTable::GetLocal(const std::string& name) {
  if (scopes_.back().count(name) != 0)
    return &scopes_.back()[name];
  return nullptr;
}

SymbolTable::SymbolTable() {
  NewScope();
}

void SymbolTable::Put(const std::shared_ptr<FuncDecl>& func_decl) {
  PutGlobal(func_decl->name_->GetStringValue(),
      func_decl->return_type_,
      func_decl->indirection_,
      0,
      true);
}

void SymbolTable::PutLocal(const std::string& name, Type type, int ind, int len, int offset) {
  scopes_.back()[name] = {type, ind, len, false, true, offset};
}

void SymbolTable::PutGlobal(const std::string& name, Type type, int ind, int len, bool is_function) {
  scopes_.front()[name] = {type, ind, len, is_function, false, 0};
}

void SymbolTable::NewScope() {
  scopes_.emplace_back();
}

void SymbolTable::EndScope() {
  scopes_.pop_back();
}

bool SymbolTable::Contains(const std::string& name) {
  return Get(name) != nullptr;
}

} // namespace mcc