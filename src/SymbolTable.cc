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

void SymbolTable::Put(const std::string& name, Type* type, Entry *fields, bool is_struct) {
  scopes_.front()[name] = {type, false, is_struct, 0, nullptr, fields};
}

void SymbolTable::Put(const std::shared_ptr<FuncDecl>& func_decl) {
  PutGlobal(func_decl->name_->String(),
      &func_decl->return_type_,
      func_decl.get(), false);
}

void SymbolTable::PutLocal(const std::string& name, Type* type, int offset) {
  scopes_.back()[name] = {type, true, false, offset, nullptr, nullptr};
}

void SymbolTable::PutGlobal(const std::string& name, Type* type, FuncDecl* func, bool is_struct) {
  scopes_.front()[name] = {type, false, is_struct, 0, func, nullptr};
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

std::unordered_map<std::string, Entry>& SymbolTable::GetGlobalScope() {
  return scopes_.front();
}

} // namespace mcc