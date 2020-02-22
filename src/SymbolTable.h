//
// Created by vladyslav on 16.01.20.
//

#ifndef MCC_SRC_SYMBOLTABLE_H_
#define MCC_SRC_SYMBOLTABLE_H_

#include "common.h"
#include "Type.h"

#define ENTRY_LIMIT 1024

namespace mcc {

class FuncDecl;

struct Entry {
  Type* type;
  bool is_local;
  int offset;
  FuncDecl* func;
  Entry* next;
  std::string name;
};

struct TypeEntry {
  Type* type;
  int size;
  Entry *next;
};

// #FIXME: redesign a better (faster, simpler) symbol table
class SymbolTable {
 public:
  SymbolTable();

  Entry* GetField(const Type& type, const std::string& field);

  bool ContainsType(const std::string& name) const;

  void PutType(const std::string& name, int size, Entry *next);

  void ClearTypes();

  TypeEntry* GetType(const std::string& name);

  Entry* Get(const std::string& name);

  Entry* GetLocal(const std::string& name);

  void Put(const std::shared_ptr<FuncDecl>& func_decl);

  void Put(const std::string &name, Type* type, Entry *fields);

  void PutType(const std::string &name, Type *type, Entry *next);

  void PutLocal(const std::string& name, Type* type, int offset);

  void PutGlobal(const std::string& name, Type* type, FuncDecl* func);

  void NewScope();

  void EndScope();

  bool Contains(const std::string& name);

  std::unordered_map<std::string, Entry>& GetGlobalScope();

 private:
  std::vector<std::unordered_map<std::string, Entry>> scopes_;

  std::unordered_map<std::string, TypeEntry> types_;
};

} // namespace mcc

#endif //MCC_SRC_SYMBOLTABLE_H_
