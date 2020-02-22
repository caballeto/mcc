//
// Created by vladyslav on 15.01.20.
//

#ifndef MCC_SRC_CODEGENX86_H_
#define MCC_SRC_CODEGENX86_H_

#include "common.h"
#include "ast.h"
#include "Visitor.h"
#include "SymbolTable.h"
#include "ControlFlowChecker.h"
#include "ErrorReporter.h"
#include "TokenType.h"

#define REGISTER_ARR_LENGTH 10
#define REGISTER_NUM 6
#define NO_RETURN_REGISTER -1

namespace mcc {

class CodeGenX86: public Visitor<int> {
 public:
  CodeGenX86(std::ostream& out, SymbolTable& symbol_table, ErrorReporter& reporter)
    : out_(out), symbol_table_(symbol_table), reporter_(reporter) {
    label_ = 1;
    return_label_ = -1;
    spilled_reg_ = 0;

    type_sizes_[TokenType::T_VOID] = 1; // for void pointers
    type_sizes_[TokenType::T_CHAR] = 1;
    type_sizes_[TokenType::T_SHORT] = 2;
    type_sizes_[TokenType::T_INT] = 4;
    type_sizes_[TokenType::T_LONG] = 8;
  }

  void Generate(const std::vector<std::shared_ptr<Stmt>>& stmts);

  int Visit(const std::shared_ptr<Binary>& binary) override;
  int Visit(const std::shared_ptr<Unary>& unary) override;
  int Visit(const std::shared_ptr<Literal>& literal) override;
  int Visit(const std::shared_ptr<Assign>& assign) override;
  int Visit(const std::shared_ptr<VarDecl>& var_decl) override;
  int Visit(const std::shared_ptr<Print>& print) override;
  int Visit(const std::shared_ptr<ExpressionStmt>& expr_stmt) override;
  int Visit(const std::shared_ptr<Conditional>& cond_stmt) override;
  int Visit(const std::shared_ptr<Block>& block_stmt) override;
  int Visit(const std::shared_ptr<While>& while_stmt) override;
  int Visit(const std::shared_ptr<For>& for_stmt) override;
  int Visit(const std::shared_ptr<DeclList>& decl_list) override;
  int Visit(const std::shared_ptr<ExprList>& expr_list) override;
  int Visit(const std::shared_ptr<ControlFlow>& flow_stmt) override;
  int Visit(const std::shared_ptr<FuncDecl>& func_decl) override;
  int Visit(const std::shared_ptr<Return>& return_stmt) override;
  int Visit(const std::shared_ptr<Call>& call) override;
  int Visit(const std::shared_ptr<Index> &index) override;
  int Visit(const std::shared_ptr<Grouping> &grouping) override;
  int Visit(const std::shared_ptr<Ternary> &ternary) override;
  int Visit(const std::shared_ptr<Postfix> &postfix) override;
  int Visit(const std::shared_ptr<Label> &label) override;
  int Visit(const std::shared_ptr<GoTo> &go_to) override;
  int Visit(const std::shared_ptr<Struct> &struct_decl) override;
  int Visit(const std::shared_ptr<Access> &access) override;
  int Visit(const std::shared_ptr<Union> &union_decl) override;
  int Visit(const std::shared_ptr<Enum> &enum_decl) override;
  int Visit(const std::shared_ptr<Switch> &switch_stmt) override;
  int Visit(const std::shared_ptr<Typedef> &typedef_stmt) override;
  int Visit(const std::shared_ptr<TypeCast> &type_cast) override;

  int GetLabel();

  void GenGlobalString(const std::string& s, int label);

  void Preamble();

  void SpillRegs();
  void UnspillRegs();
  void Spill(int r);
  void Unspill(int r);

  int NewRegister();
  void FreeRegister(int reg);
  void PrintInt(int reg);

  static std::string GetSetInstr(TokenType type);

  int GetTypeSize(const Type& type, int ind);
  static std::string GetAllocType(const Type& type, int ind);
  std::string GetSavePostfix(const Type& type, int ind);
  std::string GetLoadPostfix(const Type& type, int ind);
  std::string GetRegister(int r, const Type& type, int ind);

  static std::string GenLoad(const std::string &name, int offset, bool is_local);
  static std::string GenLoad(const std::shared_ptr<Literal> &literal);
  std::ostream& GenLabel(int label);

  void GenGlobalStrings();
  void GenGlobals();
  int GetStructSize(Entry *next);
  void GenData();
  void GenGlob(const std::string &name);
  void GenJump(int label);
  void GenLabel(const std::string &name);
  void GenText();
  void GenJumpTable(int l_jump_table, const std::vector<std::pair<int, int>> &case_labels, int l_default);
  void GenJumpToTable(int r, int l_jump_table, int l_reg_load);
  int GenCompositeSize(const std::string &name);

  // vars
  std::map<std::string, int> strings_;
  std::map<TokenType, int> type_sizes_;
  std::stack<std::pair<std::string, std::string>> loop_stack_; // FIXME: move to type checker
  std::unordered_map<std::string, std::unordered_map<std::string, int>> labels_;

  std::shared_ptr<FuncDecl> curr_func;

  int label_;
  int return_label_;
  int spilled_reg_;

  std::ostream& out_;
  ControlFlowChecker flow_checker_; // FIXME: move to type checker
  SymbolTable& symbol_table_;
  ErrorReporter& reporter_;

  const std::string kRegisters[10] = {"%r10", "%r11", "%r12", "%r13",
                                "%r9", "%r8", "%rcx", "%rdx", "%rsi", "%rdi"};
  const std::string kDregisters[10] = {"%r10d", "%r11d", "%r12d", "%r13d",
                                       "%r9d", "%r8d", "%ecx", "%edx", "%esi", "%edi"};
  const std::string kBregisters[10] = {"%r10b", "%r11b", "%r12b", "%r13b",
                                       "%r9b", "%r8b", "%cl", "%dl", "%sil", "%dil"};
  const std::string kWregisters[10] = {"%r10w", "%r11w", "%r12w", "%r13w",
                                       "%r9w", "%r8w", "%cx", "%dx", "%si", "%di"};

  bool regs_status[REGISTER_NUM] = {true, true, true, true, true, true};
};

} // namespace mcc

#endif //MCC_SRC_CODEGENX86_H_
