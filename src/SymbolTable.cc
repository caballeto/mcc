//
// Created by vladyslav on 16.01.20.
//

#include "SymbolTable.h"
#include "ast.h"

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

void SymbolTable::Put(const std::string& name, Type* type, Entry *fields) {
  scopes_.front()[name] = {type, false, 0, nullptr, fields};
}

void SymbolTable::Put(FuncDecl& func_decl) {
  PutGlobal(func_decl.name_->String(),
      &func_decl.return_type_,
      &func_decl, 0);
}

void SymbolTable::PutLocal(const std::string& name, Type* type, int offset) {
  scopes_.back()[name] = {type, true,  offset, nullptr, nullptr};
}

void SymbolTable::PutGlobal(const std::string& name, Type* type, FuncDecl* func, long init) {
  scopes_.front()[name] = {type, false,  0, func, nullptr, "", init};
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
  return scopes_[0];
}

void SymbolTable::PutType(const std::string& name, Type* type, Entry *next) {
  types_[name] = {type, 0, next};
}

void SymbolTable::PutType(const std::string& name, int size, Entry *next) {
  types_[name] = {nullptr, size, next};
}

TypeEntry* SymbolTable::GetType(const std::string& name) {
  if (!ContainsType(name)) return nullptr;
  return &types_[name];
}

bool SymbolTable::ContainsType(const std::string& name) const {
  return types_.count(name) != 0;
}

Entry* SymbolTable::GetField(const Type& type, const std::string& field) {
  if (type.type_ == TokenType::T_NONE || type.name == nullptr) return nullptr;
  Entry* fields = types_[type.name->String()].next;
  while (fields != nullptr) {
    if (fields->name == field) {
      return fields;
    }
    fields = fields->next;
  }

  return nullptr;
}

void SymbolTable::ClearTypes() {
  types_.clear();
}

} // namespace mcc