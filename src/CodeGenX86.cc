//
// Created by vladyslav on 15.01.20.
//

#include "CodeGenX86.h"

// #TODO: future optimization - don't generate code for sub-trees that have no effects
// #TODO: eliminate useless condition expression generation

namespace mcc {

// #TODO: optimize for no 'else' case (don't generate end label)
int CodeGenX86::Visit(Conditional& cond_stmt) {
  int r = cond_stmt.condition_->Accept(*this);
  int l_start = GetLabel(), l_end = GetLabel();

  out_ << "\tcmpq\t$0, " << kRegisters[r] << "\n";

  FreeRegister(r);
  out_ << "\tje\t" << "L" << l_start << "\n";
  cond_stmt.then_block_->Accept(*this);
  GenJump(l_end);
  GenLabel(l_start);

  if (cond_stmt.else_block_ != nullptr)
    cond_stmt.else_block_->Accept(*this);

  GenLabel(l_end);
  return NO_RETURN_REGISTER;
}

void CodeGenX86::GenCondJump(int r, TokenType type, int label) {
  out_ << "\ttest\t" << kRegisters[r] << ", " << kRegisters[r] << "\n";
  if (type == TokenType::T_AND) {
    out_ << "\tje\tL" << label << "\n";
  } else if (type == TokenType::T_OR) {
    out_ << "\tjne\tL" << label << "\n";
  } else {
    std::cerr << "Internal error: unexpected operator type in logical expr" << type << std::endl;
    exit(1);
  }
}

int CodeGenX86::Visit(Logical& logical) {
  TokenType op = logical.op_->GetType();
  int l_false = GetLabel(), l_end = GetLabel();

  int r = logical.left_->Accept(*this);
  GenCondJump(r, op, l_false);
  FreeRegister(r);

  r = logical.right_->Accept(*this);
  GenCondJump(r, op, l_false);

  if (op == TokenType::T_AND) {
    out_ << "\tmovq\t$1, " << kRegisters[r] << "\n";
    GenJump(l_end);
    GenLabel(l_false);
    out_ << "\tmovq\t$0, " << kRegisters[r] << "\n";
  } else {
    out_ << "\tmovq\t$0, " << kRegisters[r] << "\n";
    GenJump(l_end);
    GenLabel(l_false);
    out_ << "\tmovq\t$1, " << kRegisters[r] << "\n";
  }

  GenLabel(l_end);
  return r;
}

int CodeGenX86::Visit(While& while_stmt) {
  // check if we have break/continue statements
  bool has_control_flow_stmts = while_stmt.loop_block_->Accept(flow_checker_);
  int l_body = GetLabel(), l_condition = GetLabel(), l_end;

  // generate additional labels to allow break/continue
  if (has_control_flow_stmts) {
    l_end = GetLabel();
    loop_stack_.push({"L" + std::to_string(l_end), "L" + std::to_string(l_condition)});
  }

  if (!while_stmt.do_while_)
    GenJump(l_condition);

  GenLabel(l_body);
  while_stmt.loop_block_->Accept(*this);

  // generate condition label if there is break/continue or it is a while stmt
  if (has_control_flow_stmts || !while_stmt.do_while_)
    GenLabel(l_condition);

  int r = while_stmt.condition_->Accept(*this);
  out_ << "\tcmpq\t$0, " << kRegisters[r] << "\n";
  FreeRegister(r);
  out_ << "\tjne\t" << "L" << l_body << "\n";

  // generate end label and pop break/continue labels from stack
  if (has_control_flow_stmts) {
    GenLabel(l_end);
    loop_stack_.pop();
  }

  return NO_RETURN_REGISTER;
}

int CodeGenX86::Visit(For& for_stmt) {
  bool has_control_flow_stmts = for_stmt.loop_block_->Accept(flow_checker_);

  for_stmt.init_->Accept(*this);
  int l_body = GetLabel(), l_condition = GetLabel(), l_update, l_end;

  if (has_control_flow_stmts) {
    l_update = GetLabel();
    l_end = GetLabel();
    loop_stack_.push({"L" + std::to_string(l_end), "L" + std::to_string(l_update)});
  }

  GenJump(l_condition);
  GenLabel(l_body);
  for_stmt.loop_block_->Accept(*this);

  if (has_control_flow_stmts)
    GenLabel(l_update);

  for_stmt.update_->Accept(*this);
  GenLabel(l_condition);

  if (for_stmt.condition_ != nullptr) {
    int r = for_stmt.condition_->Accept(*this);
    out_ << "\tcmpq\t$0, " << kRegisters[r] << "\n";
    FreeRegister(r);
    out_ << "\tjne\t" << "L" << l_body << "\n";
  } else {
    GenJump(l_body);
  }

  // generate end label and pop break/continue labels from stack
  if (has_control_flow_stmts) {
    GenLabel(l_end);
    loop_stack_.pop();
  }

  return NO_RETURN_REGISTER;
}

// 1. generate switch expr
// 2. load result into jump register
// 3. generate jump table
// 4. jump to jump selector code
// 5. labels and cases
// 6. end label
int CodeGenX86::Visit(Switch& switch_stmt) {
  int l_reg_load = GetLabel(), l_jump_table = GetLabel(), l_end = GetLabel(), label, l_default = -1;
  std::vector<std::pair<int, int>> case_labels;
  int r = switch_stmt.expr_->Accept(*this);
  GenJump(l_reg_load);

  loop_stack_.push({"L" + std::to_string(l_end), ""});
  for (const auto& pair : switch_stmt.cases_) {
    label = GetLabel();
    GenLabel(label);
    if (pair.first == nullptr)
      l_default = label;
    else
      case_labels.emplace_back(pair.first->op_->Int(), label);
    pair.second->Accept(*this);
  }

  loop_stack_.pop();
  GenJump(l_end);
  GenJumpTable(l_jump_table, case_labels, l_default == -1 ? l_end : l_default);
  GenJumpToTable(r, l_jump_table, l_reg_load);
  GenLabel(l_end);
  FreeRegister(r);
  return 0;
}

void CodeGenX86::GenJumpTable(int l_jump_table, const std::vector<std::pair<int, int>>& case_labels, int l_default) {
  GenLabel(l_jump_table);
  out_ << "\t.quad\t" << case_labels.size() << "\n";
  for (const auto& pair : case_labels)
    out_ << "\t.quad\t" << pair.first << ", L" << pair.second << "\n";
  out_ << "\t.quad\tL" << l_default << "\n";
}

void CodeGenX86::GenJumpToTable(int r, int l_jump_table, int l_reg_load) {
  GenLabel(l_reg_load);
  out_ << "\tmovq\t" << kRegisters[r] << ", %rax\n";
  out_ << "\tleaq\tL" << l_jump_table << "(%rip), %rdx\n";
  out_ << "\tjmp\tswitch\n";
}

int CodeGenX86::Visit(Block& block_stmt) {
  for (const auto& stmt : block_stmt.stmts_) {
    stmt->Accept(*this);
  }
  return NO_RETURN_REGISTER;
}

// book.name = "LOTR";
int CodeGenX86::Visit(Access& access) {
  access.name_->return_ptr_ = true;
  int r = access.name_->Accept(*this);

  const std::shared_ptr<Literal>& lit = std::static_pointer_cast<Literal>(access.field_);
  out_ << "\taddq\t$" << lit->offset_ << ", " << kRegisters[r] << "\n";
  if (!access.return_ptr_) {
    out_ << "\tmov" << GetLoadPostfix(lit->type_, lit->type_.ind)
         << "\t" << "(" << kRegisters[r] << ")" << ", " << kRegisters[r] << "\n";
  }

  return r;
}

int CodeGenX86::Visit(Unary& unary) {
  int r;

  if (unary.expr_ != nullptr) {
    r = unary.expr_->Accept(*this);
  }

  switch (unary.op_->GetType()) {
    case TokenType::T_PLUS: break;
    case TokenType::T_MINUS:
      out_ << "\tnegq\t" << kRegisters[r] << "\n";
      break;
    case TokenType::T_NEG:
      out_ << "\tnotq\t" << kRegisters[r] << "\n";
      break;
    case TokenType::T_NOT:
      out_ << "\ttest\t" << kRegisters[r] << ", " << kRegisters[r] << "\n";
      out_ << "\tsete\t" << kBregisters[r] << "\n";
      out_ << "\tmovzbq\t" << kBregisters[r] << ", " << kRegisters[r] << "\n";
      break;
    case TokenType::T_INC: {
      if (unary.expr_->IsVariable()) {
        const std::shared_ptr<Literal>& lit = std::static_pointer_cast<Literal>(unary.expr_);
        out_ << "\tadd" << GetSavePostfix(unary.expr_->type_, unary.expr_->type_.ind)
             << "\t$" << (unary.to_scale_ ? type_sizes_[unary.expr_->type_.type_] : 1) << ", " << GenLoad(*lit) << "\n";
        out_ << "\tmov" << GetSavePostfix(unary.expr_->type_, unary.expr_->type_.ind) << "\t"
             << GenLoad(*lit) << ", " << GetRegister(r, unary.expr_->type_, unary.expr_->type_.ind) << "\n";
      } else {
        out_ << "\tadd" << GetSavePostfix(unary.expr_->type_, unary.expr_->type_.ind)
             << "\t$1, ("
             << kRegisters[r] << ")\n";
        out_ << "\tmov" << GetSavePostfix(unary.expr_->type_, unary.expr_->type_.ind)
             << "\t(" << kRegisters[r] << "), "
             << GetRegister(r, unary.expr_->type_, unary.expr_->type_.ind)
             << "\n";
      }
      break;
    }
    case TokenType::T_SIZEOF: {
      r = NewRegister();
      out_ << "\tmovq\t$" << unary.op_->Int() << ", " << kRegisters[r] << "\n";
      return r;
    }
    case TokenType::T_DEC: {
      if (unary.expr_->IsVariable()) {
        const std::shared_ptr<Literal>& lit = std::static_pointer_cast<Literal>(unary.expr_);
        out_ << "\tsub" << GetSavePostfix(unary.expr_->type_, unary.expr_->type_.ind)
             << "\t$" << (unary.to_scale_ ? type_sizes_[unary.expr_->type_.type_] : 1) << ", " << GenLoad(*lit) << "\n";
        out_ << "\tmov" << GetSavePostfix(unary.expr_->type_, unary.expr_->type_.ind) << "\t"
             << GenLoad(*lit) << ", " << GetRegister(r, unary.expr_->type_, unary.expr_->type_.ind) << "\n";
      } else {
        out_ << "\tsub" << GetSavePostfix(unary.expr_->type_, unary.expr_->type_.ind)
             << "\t$1, (" << kRegisters[r] << ")\n";
        out_ << "\tmov" << GetSavePostfix(unary.expr_->type_, unary.expr_->type_.ind)
             << "\t(" << kRegisters[r] << "), "
             << GetRegister(r, unary.expr_->type_, unary.expr_->type_.ind)
             << "\n";
      }
      break;
    }
    case TokenType::T_BIT_AND: {
      if (unary.expr_->IsVariable()) {
        const std::shared_ptr<Literal>& lit = std::static_pointer_cast<Literal>(unary.expr_);
        out_ << "\tleaq\t" << GenLoad(*lit) << ", " << kRegisters[r] << "\n";
      }
      break;
    }
    case TokenType::T_STAR: {
      if (!unary.return_ptr_)
        out_ << "\tmov" << GetLoadPostfix(unary.expr_->type_, unary.expr_->type_.ind - 1)
             << "\t" << "(" << kRegisters[r] << ")" << ", " << kRegisters[r] << "\n";
      break;
    }
    default: {
      std::cerr << "Invalid operator in unary expression: " << unary.op_ << std::endl;
      exit(1);
    }
  }

  return r;
}

int CodeGenX86::Visit(Binary& binary) {
  int r1 = binary.left_->Accept(*this);
  int r2 = binary.right_->Accept(*this);

  int rsize = GetTypeSize(binary.right_->type_, 0);
  int lsize = GetTypeSize(binary.left_->type_, 0);
  if (binary.left_->to_scale_ && rsize > 1)
    out_ << "\timulq\t$" << rsize << ", " << kRegisters[r1] << "\n";
  if (binary.right_->to_scale_ && lsize > 1)
    out_ << "\timulq\t$" << lsize << ", " << kRegisters[r2] << "\n";

  switch (binary.op_->GetType()) {
    case TokenType::T_PLUS:
      out_ << "\taddq\t" << kRegisters[r2] << ", " << kRegisters[r1] << "\n";
      FreeRegister(r2);
      return r1;
    case TokenType::T_MINUS:
      out_ << "\tsubq\t" << kRegisters[r2] << ", " << kRegisters[r1] << "\n";
      FreeRegister(r2);
      return r1;
    case TokenType::T_STAR:
      out_ << "\timulq\t" << kRegisters[r2] << ", " << kRegisters[r1] << "\n";
      FreeRegister(r2);
      return r1;
    case TokenType::T_SLASH:
      out_ << "\tmovq\t" << kRegisters[r1] << ", %rax\n"
           << "\tcqo\n"
           << "\tidivq\t" << kRegisters[r2] << "\n"
           << "\tmovq\t%rax, " << kRegisters[r1] << "\n";
      FreeRegister(r2);
      return r1;
    case TokenType::T_MOD:
      out_ << "\tmovq\t" << kRegisters[r1] << ", %rax\n"
           << "\tcqo\n"
           << "\tidivq\t" << kRegisters[r2] << "\n"
           << "\tmovq\t%rdx, " << kRegisters[r1] << "\n";
      FreeRegister(r2);
      return r1;
    case TokenType::T_BIT_AND:
      out_ << "\tandq\t" << kRegisters[r2] << ", " << kRegisters[r1] << "\n";
      FreeRegister(r2);
      return r1;
    case TokenType::T_BIT_OR:
      out_ << "\torq\t" << kRegisters[r2] << ", " << kRegisters[r1] << "\n";
      FreeRegister(r2);
      return r1;
    case TokenType::T_BIT_XOR:
      out_ << "\txorq\t" << kRegisters[r2] << ", " << kRegisters[r1] << "\n";
      FreeRegister(r2);
      return r1;
    case TokenType::T_LSHIFT:
      out_ << "\tmovb\t" << kBregisters[r2] << ", %cl\n";
      out_ << "\tshlq\t%cl, " << kRegisters[r1] << "\n";
      FreeRegister(r2);
      return r1;
    case TokenType::T_RSHIFT:
      out_ << "\tmovb\t" << kBregisters[r2] << ", %cl\n";
      out_ << "\tshrq\t%cl, " << kRegisters[r1] << "\n";
      FreeRegister(r2);
      return r1;
    case TokenType::T_EQUALS:
    case TokenType::T_NOT_EQUALS:
    case TokenType::T_GREATER:
    case TokenType::T_LESS:
    case TokenType::T_GREATER_EQUAL:
    case TokenType::T_LESS_EQUAL:
      out_ << "\tcmpq\t" << kRegisters[r2] << ", " << kRegisters[r1] << "\n";
      out_ << "\t" << GetSetInstr(binary.op_->GetType()) << "\t" << kBregisters[r2] << "\n";
      out_ << "\tmovzbq\t" << kBregisters[r2] << ", " << kRegisters[r1] << "\n";
      FreeRegister(r2);
      return r1;
    default:
      std::cerr << "Invalid operator in binary expression: " << binary.op_ << std::endl;
      exit(1);
  }
}

int CodeGenX86::Visit(Literal& literal) {
  int r = NewRegister();

  // order does matter below
  // arrays
  if (literal.type_.IsArray()) {
    out_ << "\tleaq\t" << GenLoad(literal) << ", " << kRegisters[r] << "\n";
    return r;
  }

  // pointers
  if (literal.type_.ind != 0 && literal.op_->GetType() != TokenType::T_STR_LIT) {
    out_ << "\tmovq\t" << GenLoad(literal) << ", " << kRegisters[r] << "\n";
    return r;
  }

  // structs
  if (literal.type_.IsStruct() || literal.type_.IsUnion()) {
    out_ << "\tleaq\t" << GenLoad(literal) << ", " << kRegisters[r] << "\n";
    return r;
  }

  // primitives
  switch (literal.op_->GetType()) {
    case TokenType::T_INT_LIT:
      out_ << "\tmovq\t$" << literal.op_->Int()
           << "," << kRegisters[r] << "\n";
      break;
    case TokenType::T_STR_LIT: {
      const std::string &s = literal.op_->String();
      out_ << "\tleaq\tL" << strings_[s] << "(%rip), " << kRegisters[r] << "\n";
      break;
    }
    case TokenType::T_IDENTIFIER:
      switch (literal.type_.type_) {
        case TokenType::T_CHAR:
          out_ << "\tmovzbq\t" << GenLoad(literal) << ", " << kRegisters[r] << "\n";
          break;
        case TokenType::T_SHORT:
          out_ << "\tmovzwq\t" << GenLoad(literal) << ", " << kRegisters[r] << "\n";
          break;
        case TokenType::T_INT:
          out_ << "\tmovslq\t" << GenLoad(literal) << ", " << kRegisters[r] << "\n";
          break;
        case TokenType::T_LONG:
          out_ << "\tmovq\t" << GenLoad(literal) << ", " << kRegisters[r] << "\n";
          break;
        default: {
          std::cerr << "Invalid literal type. Exiting." << std::endl;
          std::cout << literal.type_ << std::endl;
          std::cout << literal.op_->String() << std::endl;
          exit(1);
        }
      }
      break;
    default:
      std::cerr << "Invalid literal type '" << literal.op_->GetType()
        << "'." << std::endl;
      exit(1);
  }

  return r;
}

int CodeGenX86::Visit(Print& print) {
  int r = print.expr_->Accept(*this);
  PrintInt(r);
  FreeRegister(r);
  return NO_RETURN_REGISTER;
}

#define ASSIGN_OP(op)       \
      if (assign.left_->IsVariable()) { \
        const std::shared_ptr<Literal>& lit = std::static_pointer_cast<Literal>(assign.left_); \
        out_ << "\t" op << GetSavePostfix(assign.left_->type_, assign.left_->type_.ind) << "\t" \
             << GetRegister(r1, assign.left_->type_, assign.left_->type_.ind) \
             << ", " << GenLoad(*lit) << "\n"; \
      } else { \
        int r2 = assign.left_->Accept(*this); \
        out_ << "\t" op << GetSavePostfix(assign.left_->type_, assign.left_->type_.ind) << "\t" \
             << GetRegister(r1, assign.left_->type_, assign.left_->type_.ind) \
             << ", (" << kRegisters[r2] << ")\n"; \
             FreeRegister(r2); \
      }

#define RAX -1
#define RDX -2

// #FIXME: pass <Literal> as type, or cast <Expr> to here?
int CodeGenX86::Visit(Assign& assign) {
  int r1 = assign.right_->Accept(*this);

  switch (assign.op_->GetType()) {
    case TokenType::T_ASSIGN:
      ASSIGN_OP("mov")
      break;
    case TokenType::T_ASSIGN_PLUS:
      ASSIGN_OP("add")
      break;
    case TokenType::T_ASSIGN_MINUS:
      ASSIGN_OP("sub")
      break;
    case TokenType::T_ASSIGN_MUL: {
      const std::string& postfix = GetSavePostfix(assign.left_->type_, assign.left_->type_.ind);
      const std::string& reg = GetRegister(r1, assign.left_->type_, assign.left_->type_.ind);
      if (assign.left_->IsVariable()) {
        const std::shared_ptr<Literal> &lit = std::static_pointer_cast<Literal>(assign.left_);
        out_ << "\timul" << postfix << "\t" << GenLoad(*lit) << ", " << reg << "\n";
        out_ << "\tmov" << postfix << "\t" << reg << ", "
             << GenLoad(*lit) << "\n";
      } else {
        int r2 = assign.left_->Accept(*this);
        out_ << "\timul" << postfix << "\t(" << kRegisters[r2] << "), " << reg << "\n";
        out_ << "\tmov" << postfix << "\t" << reg << ", (" << kRegisters[r2] << ")\n";
        FreeRegister(r2);
      }
      break;
    }
    case TokenType::T_ASSIGN_DIV: {
      const std::string& postfix = GetSavePostfix(assign.left_->type_, assign.left_->type_.ind);
      const std::string& rax = GetRegister(RAX, assign.left_->type_, assign.left_->type_.ind);
      if (assign.left_->IsVariable()) {
        const std::shared_ptr<Literal> &lit = std::static_pointer_cast<Literal>(assign.left_);
        out_ << "\tmov" << GetLoadPostfix(assign.left_->type_, assign.left_->type_.ind) << "\t" << GenLoad(*lit)
             << ", %rax\n" << "\tcqo\n";
        out_ << "\tidivq\t" << kRegisters[r1] << "\n";
        out_ << "\tmov" << postfix << "\t" << rax << ", " << GenLoad(*lit) << "\n";
        out_ << "\tmovq\t%rax, " << kRegisters[r1] << "\n";
      } else {
        int r2 = assign.left_->Accept(*this);
        out_ << "\tmov" << GetLoadPostfix(assign.left_->type_, assign.left_->type_.ind) << "\t(" << kRegisters[r2] << ")"
             << ", %rax\n" << "\tcqo\n";
        out_ << "\tidivq\t" << kRegisters[r1] << "\n";
        out_ << "\tmov" << postfix << "\t" << rax << ", (" << kRegisters[r2] << ")\n";
        out_ << "\tmovq\t%rax, " << kRegisters[r1] << "\n";
        FreeRegister(r2);
      }
      break;
    }
    case TokenType::T_ASSIGN_MOD: {
      const std::string& postfix = GetSavePostfix(assign.left_->type_, assign.left_->type_.ind);
      const std::string& rdx = GetRegister(RDX, assign.left_->type_, assign.left_->type_.ind);
      if (assign.left_->IsVariable()) {
        const std::shared_ptr<Literal> &lit = std::static_pointer_cast<Literal>(assign.left_);
        out_ << "\tmov" << GetLoadPostfix(assign.left_->type_, assign.left_->type_.ind) << "\t" << GenLoad(*lit)
             << ", %rax\n" << "\tcqo\n";
        out_ << "\tidivq\t" << kRegisters[r1] << "\n";
        out_ << "\tmov" << postfix << "\t" << rdx << ", " << GenLoad(*lit) << "\n";
        out_ << "\tmovq\t%rdx, " << kRegisters[r1] << "\n";
      } else {
        int r2 = assign.left_->Accept(*this);
        out_ << "\tmov" << GetLoadPostfix(assign.left_->type_, assign.left_->type_.ind) << "\t(" << kRegisters[r2] << ")"
             << ", %rax\n" << "\tcqo\n";
        out_ << "\tidivq\t" << kRegisters[r1] << "\n";
        out_ << "\tmov" << postfix << "\t" << rdx << ", (" << kRegisters[r2] << ")\n";
        out_ << "\tmovq\t%rdx, " << kRegisters[r1] << "\n";
        FreeRegister(r2);
      }
      break;
    }
    case TokenType::T_ASSIGN_LSHIFT: {
      const std::string& postfix = GetSavePostfix(assign.left_->type_, assign.left_->type_.ind);
      const std::string& reg = GetRegister(r1, assign.left_->type_, assign.left_->type_.ind);
      out_ << "\tmovb\t" << kBregisters[r1] << ", %cl\n";
      if (assign.left_->IsVariable()) {
        const std::shared_ptr<Literal> &lit = std::static_pointer_cast<Literal>(assign.left_);
        out_ << "\tshlq\t%cl, " << GenLoad(*lit) << "\n";
      } else {
        int r2 = assign.left_->Accept(*this);
        out_ << "\tshlq\t%cl, (" << kRegisters[r2] << ")\n";
        FreeRegister(r2);
      }
      break;
    }
    case TokenType::T_ASSIGN_RSHIFT:{
      const std::string& postfix = GetSavePostfix(assign.left_->type_, assign.left_->type_.ind);
      const std::string& reg = GetRegister(r1, assign.left_->type_, assign.left_->type_.ind);
      out_ << "\tmovb\t" << kBregisters[r1] << ", %cl\n";
      if (assign.left_->IsVariable()) {
        const std::shared_ptr<Literal> &lit = std::static_pointer_cast<Literal>(assign.left_);
        out_ << "\tshrq\t%cl, " << GenLoad(*lit) << "\n";
      } else {
        int r2 = assign.left_->Accept(*this);
        out_ << "\tshrq\t%cl, (" << kRegisters[r2] << ")\n";
        FreeRegister(r2);
      }
      break;
    }
    case TokenType::T_ASSIGN_AND:
      ASSIGN_OP("and")
      break;
    case TokenType::T_ASSIGN_XOR:
      ASSIGN_OP("xor")
      break;
    case TokenType::T_ASSIGN_OR:
      ASSIGN_OP("or")
      break;
    default: {
      std::cerr << "Invalid assign operation " << assign.op_->GetType() << std::endl;
      exit(1);
    }
  }

  return r1;
}

int CodeGenX86::Visit(DeclList& decl_list) {
  for (const auto& var_decl : decl_list.var_decl_list_)
    var_decl->Accept(*this);
  return NO_RETURN_REGISTER;
}

int CodeGenX86::Visit(ExprList& expr_list) {
  for (const auto& expr : expr_list.expr_list_)
    FreeRegister(expr->Accept(*this));
  return NO_RETURN_REGISTER;
}

int CodeGenX86::GetTypeSize(const Type& type, int indirection) {
  if (indirection > 0) return 8;
  return type_sizes_[type.type_];
}

std::string CodeGenX86::GetLoadPostfix(const Type& type, int ind) {
  if (ind > 0) return "q";
  switch (type.type_) {
    case TokenType::T_CHAR: return "zbq";
    case TokenType::T_SHORT: return "zwq";
    case TokenType::T_INT: return "slq";
    case TokenType::T_LONG: return "q";
    default: {
      reporter_.Report("Invalid variable type.");
      exit(1);
    }
  }
}

std::string CodeGenX86::GetSavePostfix(const Type& type, int ind) {
  if (ind > 0) return "q";
  switch (type.type_) {
    case TokenType::T_CHAR: return "b";
    case TokenType::T_SHORT: return "w";
    case TokenType::T_INT: return "l";
    case TokenType::T_LONG: return "q";
    default: {
      reporter_.Report("Invalid variable type.");
      exit(1);
    }
  }
}

// #TODO: BS. Rewrite more compact with array indexes
std::string CodeGenX86::GetRegister(int r, const Type& type, int ind) {
  if (r == RAX) {
    if (ind > 0) return "%rax";
    switch (type.type_) {
      case TokenType::T_CHAR: return "%al";
      case TokenType::T_SHORT: return "%ax";
      case TokenType::T_INT: return "%eax";
      case TokenType::T_LONG: return "%rax";
      default: {
        reporter_.Report("Invalid variable type.");
        exit(1);
      }
    }
  } else if (r == RDX) {
    if (ind > 0) return "%rdx";
    switch (type.type_) {
      case TokenType::T_CHAR: return "%dl";
      case TokenType::T_SHORT: return "%dx";
      case TokenType::T_INT: return "%edx";
      case TokenType::T_LONG: return "%rdx";
      default: {
        reporter_.Report("Invalid variable type.");
        exit(1);
      }
    }
  } else {
    if (ind > 0) return kRegisters[r];
    switch (type.type_) {
      case TokenType::T_CHAR: return kBregisters[r];
      case TokenType::T_SHORT: return kWregisters[r];
      case TokenType::T_INT: return kDregisters[r];
      case TokenType::T_LONG: return kRegisters[r];
      default: {
        reporter_.Report("Invalid variable type.");
        exit(1);
      }
    }
  }
}

int CodeGenX86::Visit(VarDecl& var_decl) {
  const std::string& name = var_decl.name_->String();

  if (var_decl.is_local_ && !var_decl.var_type_.IsArray() && var_decl.init_ != nullptr) {
    int r = var_decl.init_->Accept(*this);
    out_ << "\tmov" << GetSavePostfix(var_decl.var_type_, var_decl.var_type_.ind) << "\t"
         << GetRegister(r, var_decl.var_type_, var_decl.var_type_.ind)
         << ", " << GenLoad(name, var_decl.offset_, var_decl.is_local_) << "\n";
    FreeRegister(r);
  }

  return NO_RETURN_REGISTER;
}

int CodeGenX86::Visit(Enum &enum_decl) {
  return NO_RETURN_REGISTER;
}

int CodeGenX86::Visit(Union &union_decl) {
  return NO_RETURN_REGISTER;
}

int CodeGenX86::Visit(Struct &struct_decl) {
  return NO_RETURN_REGISTER;
}

int CodeGenX86::Visit(FuncDecl& func_decl) {
  if (func_decl.body_ == nullptr) return NO_RETURN_REGISTER;
  GenText();

  // glob marker (for non-static functions)
  if (func_decl.return_type_.storage != S_STATIC) {
    GenGlob(func_decl.name_->String());
  }

  // prologue
  out_ << "\t.type\t" << func_decl.name_->String() << ", @function\n";
  out_ << func_decl.name_->String() << ":\n";
  out_ << "\tpushq\t%rbp\n";
  out_ << "\tmovq\t%rsp, %rbp\n";
  int stack_offset = (func_decl.local_offset_ + 15) & ~15;
  out_ << "\tsubq\t$" << stack_offset << ", %rsp\n";

  // parameters
  int size = func_decl.signature_->var_decl_list_.size();
  int preg = REGISTER_ARR_LENGTH - 1;
  for (int i = 0; i < std::min(size, 6); i++) {
    const auto& decl = func_decl.signature_->var_decl_list_[i];
    out_ << "\tmov" << GetSavePostfix(decl->var_type_, decl->var_type_.ind) << "\t"
         << GetRegister(preg--, decl->var_type_, decl->var_type_.ind) << ", " << decl->offset_ << "(%rbp)\n";
  }

  // body
  curr_func = &func_decl;
  return_label_ = GetLabel();
  func_decl.body_->Accept(*this);
  curr_func = nullptr;

  // epilogue
  GenLabel(return_label_);
  out_ << "\taddq\t$" << stack_offset << ", %rsp\n";
  out_ << "\tpopq\t%rbp\n";
  out_ << "\tret\n";

  return NO_RETURN_REGISTER;
}

int CodeGenX86::Visit(Return& return_stmt) {
  int r = return_stmt.expr_->Accept(*this);
  out_ << "\tmovq\t" << kRegisters[r] << ", %rax\n"; // #FIXME: add typed move?
  GenJump(return_label_);
  FreeRegister(r);
  return NO_RETURN_REGISTER;
}

int CodeGenX86::Visit(ExpressionStmt& expr_stmt) {
  int r = expr_stmt.expr_->Accept(*this);
  FreeRegister(r);
  return NO_RETURN_REGISTER;
}

int CodeGenX86::Visit(Grouping& grouping) {
  return grouping.expr_->Accept(*this);
}

// array[2] = 15;
// print array[2];
int CodeGenX86::Visit(Index& index) {
  index.name_->return_ptr_ = true;

  int r1 = index.name_->Accept(*this);
  int r2 = index.index_->Accept(*this);

  // scale the index
  int ind = index.name_->type_.ind;
  ind += index.name_->type_.IsArray() ? 0 : -1; // fix for pointers

  out_ << "\timulq\t$" << GetTypeSize(index.name_->type_, ind) << ", " << kRegisters[r2] << "\n";
  out_ << "\taddq\t" << kRegisters[r2] << ", " << kRegisters[r1] << "\n";
  FreeRegister(r2);

  if (!index.return_ptr_) {
    out_ << "\tmov" << GetLoadPostfix(index.name_->type_, ind) // #FIXME: check if name type is type + *
         << "\t" << "(" << kRegisters[r1] << ")" << ", " << kRegisters[r1] << "\n";
  }

  return r1;
}

int CodeGenX86::Visit(Call& call) {
  int size = call.args_->expr_list_.size(), r;
  SpillRegs();

  for (int i = 0; i < size; i++) {
    if (i < 6) {
      const auto& arg = call.args_->expr_list_[i];
      r = arg->Accept(*this);
      out_ << "\tmovq" << "\t" << kRegisters[r] << ", "
           << kRegisters[REGISTER_ARR_LENGTH - i - 1] << "\n";
    } else {
      const auto& arg = call.args_->expr_list_[size - i + 5]; // push in reverse order
      r = arg->Accept(*this);
      out_ << "\tpushq\t" << kRegisters[r] << "\n";
    }

    FreeRegister(r);
  }

  out_ << "\tcall\t" << std::static_pointer_cast<Literal>(call.name_)->op_->String() << "\n";
  if (size > 6)
    out_ << "\taddq\t$" << (size - 6) * 8 << ", %rsp\n";

  UnspillRegs();
  if (!call.type_.IsVoid()) {
    r = NewRegister();
    out_ << "\tmovq\t%rax, " << kRegisters[r] << "\n";
    return r;
  }

  return NO_RETURN_REGISTER;
}

int CodeGenX86::Visit(ControlFlow& flow_stmt) {
  if (loop_stack_.empty()) {
    std::cerr << ((flow_stmt.is_break_) ? "break" : "continue") << " does not have enclosing loop." << std::endl;
    exit(1);
  }

  const auto& labels = loop_stack_.top();
  out_ << "\tjmp\t" << (flow_stmt.is_break_ ? labels.first : labels.second) << "\n";
  return NO_RETURN_REGISTER;
}

int CodeGenX86::Visit(Typedef &typedef_stmt) {
  return NO_RETURN_REGISTER;
}


int CodeGenX86::Visit(TypeCast &type_cast) {
  int r = type_cast.expr_->Accept(*this);
  return r;
}

int CodeGenX86::Visit(Label& label) {
  GenLabel(label.label_);
  return NO_RETURN_REGISTER;
}

int CodeGenX86::Visit(GoTo& go_to) {
  GenJump(labels_[curr_func->name_->String()][go_to.token_->String()]);
  return NO_RETURN_REGISTER;
}

int CodeGenX86::Visit(Ternary& ternary) {
  int r = ternary.condition_->Accept(*this);
  int l_start = GetLabel(), l_end = GetLabel();

  out_ << "\tcmpq\t$0, " << kRegisters[r] << "\n";

  out_ << "\tje\t" << "L" << l_start << "\n";
  int r1 = ternary.then_->Accept(*this);
  out_ << "\tmovq\t" << kRegisters[r1] << ", " << kRegisters[r] << "\n";
  FreeRegister(r1);

  GenJump(l_end);
  GenLabel(l_start);
  int r2 = ternary.else_->Accept(*this);
  out_ << "\tmovq\t" << kRegisters[r2] << ", " << kRegisters[r] << "\n";
  FreeRegister(r2);

  GenLabel(l_end);
  return r;
}

int CodeGenX86::Visit(Postfix& postfix) {
  int r1 = postfix.expr_->Accept(*this);

  switch (postfix.op_->GetType()) {
    case TokenType::T_INC: {
      if (postfix.expr_->IsVariable()) {
        const std::shared_ptr<Literal>& lit = std::static_pointer_cast<Literal>(postfix.expr_);
        out_ << "\tadd" << GetSavePostfix(postfix.expr_->type_, postfix.expr_->type_.ind)
             << "\t$" << (postfix.to_scale_ ? type_sizes_[postfix.expr_->type_.type_] : 1)
             << ", " << GenLoad(*lit) << "\n";
        break;
      } else {
        int r2 = NewRegister();
        out_ << "\tmovq\t" << kRegisters[r1] << ", " << kRegisters[r2] << "\n";
        out_ << "\tmov" << GetSavePostfix(postfix.expr_->type_, postfix.expr_->type_.ind)
             << "\t(" << kRegisters[r2] << "), "
             << GetRegister(r1, postfix.expr_->type_, postfix.expr_->type_.ind)
             << "\n";
        out_ << "\tadd"
             << GetSavePostfix(postfix.expr_->type_, postfix.expr_->type_.ind)
             << "\t$1, (" << kRegisters[r2] << ")\n";
        FreeRegister(r2);
        return r1;
      }
    }
    case TokenType::T_DEC: {
      if (postfix.expr_->IsVariable()) {
        const std::shared_ptr<Literal>& lit = std::static_pointer_cast<Literal>(postfix.expr_);
        out_ << "\tsub" << GetSavePostfix(postfix.expr_->type_, postfix.expr_->type_.ind)
             << "\t$" << (postfix.to_scale_ ? type_sizes_[postfix.expr_->type_.type_] : 1)
             << ", " << GenLoad(*lit) << "\n";
        break;
      } else {
        int r2 = NewRegister();
        out_ << "\tmovq\t" << kRegisters[r1] << ", " << kRegisters[r2] << "\n";
        out_ << "\tmov" << GetSavePostfix(postfix.expr_->type_, postfix.expr_->type_.ind)
             << "\t(" << kRegisters[r2] << "), "
             << GetRegister(r1, postfix.expr_->type_, postfix.expr_->type_.ind)
             << "\n";
        out_ << "\tsub"
             << GetSavePostfix(postfix.expr_->type_, postfix.expr_->type_.ind)
             << "\t$1, (" << kRegisters[r2] << ")\n";
        FreeRegister(r2);
        return r1;
      }
    }
    default: {
      std::cerr << "Invalid operator in postfix expression." << std::endl;
      exit(1);
    }
  }

  return r1;
}

std::string CodeGenX86::GetSetInstr(TokenType type) {
  switch (type) {
    case TokenType::T_EQUALS: return "sete";
    case TokenType::T_NOT_EQUALS: return "setne";
    case TokenType::T_GREATER: return "setg";
    case TokenType::T_LESS: return "setl";
    case TokenType::T_GREATER_EQUAL: return "setge";
    case TokenType::T_LESS_EQUAL: return "setle";
    default:
      std::cerr << "Invalid input for recovery of set instruction " << type << std::endl;
      exit(1);
  }
}

void CodeGenX86::Generate(const std::vector<std::shared_ptr<Stmt>>& stmts) {
  Preamble();
  GenGlobalStrings();
  GenGlobals();
  for (const auto& stmt : stmts)
    stmt->Accept(*this);
}

void CodeGenX86::PrintInt(int r) {
  out_ << "\tmovq\t" << kRegisters[r] << ", %rdi\n"
    << "\tcall\tprintint\n";
}

// when all registers are allocated, spill one register on stack
// when to load it back? -> when it has been freed.
int CodeGenX86::NewRegister() {
  for (int i = 0; i < REGISTER_NUM; i++) {
    if (regs_status[i]) {
      regs_status[i] = false;
      return i;
    }
  }

  int r = (spilled_reg_ % REGISTER_NUM);
  spilled_reg_++;
  Spill(r);
  return r;
}

void CodeGenX86::FreeRegister(int reg) {
  if (reg == NO_RETURN_REGISTER) return;
  if (spilled_reg_ > 0) {
    spilled_reg_--;
    reg = (spilled_reg_ % REGISTER_NUM);
    Unspill(reg);
  } else {
    regs_status[reg] = true;
  }
}

int CodeGenX86::GetLabel() {
  return label_++;
}

int CodeGenX86::GenCompositeSize(const std::string& name) {
  return symbol_table_.GetType(name)->size;
}

int CodeGenX86::GetStructSize(Entry* next) {
  if (next == nullptr) return 0;
  while (next->next != nullptr) {
    next = next->next;
  }
  return std::abs(next->offset) + GetTypeSize(*next->type, next->type->ind);
}

void CodeGenX86::GenText() {
  if (segment_ != Segment::TEXT) {
    out_ << "\t.text\n";
    segment_ = Segment::TEXT;
  }
}

void CodeGenX86::GenData() {
  if (segment_ != Segment::DATA) {
    out_ << "\t.data\n";
    segment_ = Segment::DATA;
  }
}

void CodeGenX86::GenGlob(const std::string& name) {
  out_ << "\t.globl\t" << name << "\n";
}

void CodeGenX86::GenJump(int label) {
  out_ << "\tjmp\tL" << label << "\n";
}

void CodeGenX86::GenLabel(const std::string& name) {
  out_ << name << ":\n";
}

void CodeGenX86::GenGlobals() {
  std::unordered_map<std::string, Entry>& global = symbol_table_.GetGlobalScope();
  for (const auto& pair : global) {
    if (pair.second.func != nullptr || pair.second.type->type_ == TokenType::T_ENUM || pair.second.type->storage == S_EXTERN)
      continue;
    const std::string& name = pair.first;
    const Entry& entry = pair.second;
    if (entry.type->IsArray()) {
      GenData();
      if (entry.type->storage != S_STATIC) GenGlob(name);
      GenLabel(name);
      for (int i = 0; i < entry.type->len; i++) {
        out_ << "\t." << GetAllocType(*entry.type, entry.type->ind) << "\t0\n";
      }
    } else if (entry.type->IsPointer() || entry.type->IsPrimitive()) {
      GenData();
      if (entry.type->storage != S_STATIC)
        GenGlob(name);
      GenLabel(name);
      out_ << "\t." << GetAllocType(*entry.type, entry.type->ind)
           << "\t" << entry.init << "\n";
    } else if (entry.type->IsStruct() || entry.type->IsUnion()) { // struct variable
      GenData();
      if (entry.type->storage != S_STATIC) GenGlob(name);
      GenLabel(name);
      int size = GenCompositeSize(entry.type->name->String());
      for (int i = 0; i < size; i++) {
        out_ << "\t.byte\t0\n";
      }
    }
  }
}

void CodeGenX86::GenGlobalStrings() {
  GenData();
  for (const auto& pair : strings_) {
    int label = GetLabel();
    strings_[pair.first] = label;
    GenGlobalString(pair.first, label);
  }
}

void CodeGenX86::GenGlobalString(const std::string& s, int label) {
  GenLabel(label);
  for (char c : s)
    out_ << "\t.byte\t" << (int) c << "\n";
  out_ << "\t.byte\t0\n";
}

std::string CodeGenX86::GenLoad(Literal& literal) {
  return GenLoad(literal.op_->String(), literal.offset_, literal.is_local_);
}

std::string CodeGenX86::GenLoad(const std::string& name, int offset, bool is_local) {
  if (is_local) {
    return std::to_string(offset) + "(%rbp)";
  } else {
    return name + "(%rip)";
  }
}

std::ostream& CodeGenX86::GenLabel(int label) {
  out_ << "L" << label << ":\n";
  return out_;
}

std::string CodeGenX86::GetAllocType(const Type& type, int ind) {
  if (ind > 0) return "quad";
  switch (type.type_) {
    case TokenType::T_CHAR: return "byte";
    case TokenType::T_SHORT: return "word";
    case TokenType::T_INT: return "long";
    case TokenType::T_LONG: return "quad";
    default: {
      std::cerr << "Invalid type in array allocation." << type << std::endl;
      exit(1);
    }
  }
}
void CodeGenX86::SpillRegs() {
  for (int i = 0; i < REGISTER_NUM; i++)
    Spill(i);
}

void CodeGenX86::UnspillRegs() {
  for (int i = REGISTER_NUM - 1; i >= 0; i--)
    Unspill(i);
}

void CodeGenX86::Spill(int r) {
  out_ << "\tpushq\t" << kRegisters[r] << "\n";
}

void CodeGenX86::Unspill(int r) {
  out_ << "\tpopq\t" << kRegisters[r] << "\n";
}
void CodeGenX86::Preamble() {
  GenText();
  out_ << "switch:\n"
       << "\tpushq\t%rsi\n"
       << "\tmovq\t%rdx,%rsi\n"
       << "\tmovq\t%rax,%rbx\n"
       << "\tcld\n"
       << "\tlodsq\n"
       << "\tmovq\t%rax,%rcx\n"
       << "next:\n"
       << "\tlodsq\n"
       << "\tmovq\t%rax,%rdx\n"
       << "\tlodsq\n"
       << "\tcmpq\t%rdx,%rbx\n"
       << "\tjnz\tno\n"
       << "\tpopq\t%rsi\n"
       << "\tjmp\t*%rax\n"
       << "no:\n"
       << "\tloop\tnext\n"
       << "\tlodsq\n"
       << "\tpopq\t%rsi\n"
       << "\tjmp\t*%rax\n";
}

} // namespace mcc
