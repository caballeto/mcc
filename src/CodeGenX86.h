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

enum class Segment {
  TEXT,
  DATA,
  NONE
};

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

  int Visit(Binary& binary) override;
  int Visit(Unary& unary) override;
  int Visit(Literal& literal) override;
  int Visit(Assign& assign) override;
  int Visit(VarDecl& var_decl) override;
  int Visit(Print& print) override;
  int Visit(ExpressionStmt& expr_stmt) override;
  int Visit(Conditional& cond_stmt) override;
  int Visit(Block& block_stmt) override;
  int Visit(While& while_stmt) override;
  int Visit(For& for_stmt) override;
  int Visit(DeclList& decl_list) override;
  int Visit(ExprList& expr_list) override;
  int Visit(ControlFlow& flow_stmt) override;
  int Visit(FuncDecl& func_decl) override;
  int Visit(Return& return_stmt) override;
  int Visit(Call& call) override;
  int Visit(Index &index) override;
  int Visit(Grouping &grouping) override;
  int Visit(Ternary &ternary) override;
  int Visit(Postfix &postfix) override;
  int Visit(Label &label) override;
  int Visit(GoTo &go_to) override;
  int Visit(Struct &struct_decl) override;
  int Visit(Access &access) override;
  int Visit(Union &union_decl) override;
  int Visit(Enum &enum_decl) override;
  int Visit(Switch &switch_stmt) override;
  int Visit(Typedef &typedef_stmt) override;
  int Visit(TypeCast &type_cast) override;

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
  static std::string GenLoad(Literal& literal);
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

  FuncDecl* curr_func;

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

  Segment segment_ = Segment::NONE;
};

} // namespace mcc

#endif //MCC_SRC_CODEGENX86_H_
