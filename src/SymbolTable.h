//
// Created by vladyslav on 16.01.20.
//

#ifndef MCC_SRC_SYMBOLTABLE_H_
#define MCC_SRC_SYMBOLTABLE_H_

#include "common.h"
#include "Type.h"
#include "ast.h"

#define ENTRY_LIMIT 1024

namespace mcc {

struct Entry {
  Type* type;
  bool is_local;
  int offset;
  FuncDecl* func;
  Entry* next;
};

// #FIXME: redesign a better (faster, simpler) symbol table
class SymbolTable {
 public:
  SymbolTable();

  Entry* Get(const std::string& name);

  Entry* GetLocal(const std::string& name);

  void Put(const std::shared_ptr<FuncDecl>& func_decl);

  void Put(const std::string &name, Type& type, Entry *fields);

  void PutLocal(const std::string& name, Type* type, int offset);

  void PutGlobal(const std::string& name, Type* type, FuncDecl* func);

  void NewScope();

  void EndScope();

  bool Contains(const std::string& name);

 private:
  std::vector<std::unordered_map<std::string, Entry>> scopes_;
};

} // namespace mcc

#endif //MCC_SRC_SYMBOLTABLE_H_
