//
// Created by vladyslav on 15.01.20.
//

#include "CodeGenX86.h"

// #TODO: future optimization - don't generate code for sub-trees that have no effects

namespace mcc {

// #TODO: optimize for no 'else' case (don't generate end label)
int CodeGenX86::Visit(const std::shared_ptr<Conditional>& cond_stmt) {
  int r = cond_stmt->condition_->Accept(*this);
  int l_start = GetLabel(), l_end = GetLabel();
  out_ << "\tcmpq\t$0, " << kRegisters[r] << "\n";
  FreeRegister(r);
  out_ << "\tje\t" << "L" << l_start << "\n";
  cond_stmt->then_block_->Accept(*this);
  out_ << "\tjmp\t" << "L" << l_end << "\n";
  out_ << "L" << l_start << ":\n";

  if (cond_stmt->else_block_ != nullptr)
    cond_stmt->else_block_->Accept(*this);

  out_ << "L" << l_end << ":\n";
  return NO_RETURN_REGISTER;
}

int CodeGenX86::Visit(const std::shared_ptr<While>& while_stmt) {
  // check if we have break/continue statements
  bool has_control_flow_stmts = while_stmt->loop_block_->Accept(flow_checker_);
  int l_body = GetLabel(), l_condition = GetLabel(), l_end;

  // generate additional labels to allow break/continue
  if (has_control_flow_stmts) {
    l_end = GetLabel();
    loop_stack_.push({"L" + std::to_string(l_end), "L" + std::to_string(l_condition)});
  }

  if (!while_stmt->do_while_)
    out_ << "\tjmp\t" << "L" << l_condition << "\n";

  out_ << "L" << l_body << ":\n";
  while_stmt->loop_block_->Accept(*this);

  // generate condition label if there is break/continue or it is a while stmt
  if (has_control_flow_stmts || !while_stmt->do_while_)
    out_ << "L" << l_condition << ":\n";

  int r = while_stmt->condition_->Accept(*this);
  out_ << "\tcmpq\t$0, " << kRegisters[r] << "\n";
  FreeRegister(r);
  out_ << "\tjne\t" << "L" << l_body << "\n";

  // generate end label and pop break/continue labels from stack
  if (has_control_flow_stmts) {
    out_ << "L" << l_end << ":\n";
    loop_stack_.pop();
  }

  return NO_RETURN_REGISTER;
}

int CodeGenX86::Visit(const std::shared_ptr<For>& for_stmt) {
  bool has_control_flow_stmts = for_stmt->loop_block_->Accept(flow_checker_);

  for_stmt->init_->Accept(*this);
  int l_body = GetLabel(), l_condition = GetLabel(), l_update, l_end;

  if (has_control_flow_stmts) {
    l_update = GetLabel();
    l_end = GetLabel();
    loop_stack_.push({"L" + std::to_string(l_end), "L" + std::to_string(l_update)});
  }

  out_ << "\tjmp\t" << "L" << l_condition << "\n";
  out_ << "L" << l_body << ":\n";
  for_stmt->loop_block_->Accept(*this);

  if (has_control_flow_stmts)
    out_ << "L" << l_update << ":\n";

  for_stmt->update_->Accept(*this);
  out_ << "L" << l_condition << ":\n";

  if (for_stmt->condition_ != nullptr) {
    int r = for_stmt->condition_->Accept(*this);
    out_ << "\tcmpq\t$0, " << kRegisters[r] << "\n";
    FreeRegister(r);
    out_ << "\tjne\t" << "L" << l_body << "\n";
  } else {
    out_ << "\tjmp\t" << "L" << l_body << "\n";
  }

  // generate end label and pop break/continue labels from stack
  if (has_control_flow_stmts) {
    out_ << "L" << l_end << ":\n";
    loop_stack_.pop();
  }

  return NO_RETURN_REGISTER;
}

int CodeGenX86::Visit(const std::shared_ptr<Block>& block_stmt) {
  for (const auto& stmt : block_stmt->stmts_) {
    stmt->Accept(*this);
  }
  return NO_RETURN_REGISTER;
}

int CodeGenX86::Visit(const std::shared_ptr<Unary>& unary) {
  int r = unary->right_->Accept(*this);

  switch (unary->op_->GetType()) {
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
      if (unary->right_->IsVariable()) {
        const std::string& name = std::static_pointer_cast<Literal>(unary->right_)->op_->GetStringValue();
        out_ << "\tadd" << GetPostfix(unary->right_->type_, unary->right_->indirection_)
             << "\t$" << (unary->to_scale_ ? type_sizes_[unary->right_->type_] : 1) << ", " << name << "(%rip)\n";
        out_ << "\tmov" << GetPostfix(unary->right_->type_, unary->right_->indirection_) << "\t"
             << name << "(%rip), " << GetRegister(r, unary->right_->type_, unary->right_->indirection_) << "\n";
      } else {
        out_ << "\tadd" << GetPostfix(unary->right_->type_, unary->right_->indirection_)
             << "\t$1, ("
             << kRegisters[r] << ")\n";
        out_ << "\tmov" << GetPostfix(unary->right_->type_, unary->right_->indirection_)
             << "\t(" << kRegisters[r] << "), "
             << GetRegister(r, unary->right_->type_, unary->right_->indirection_)
             << "\n";
      }
      break;
    }
    case TokenType::T_DEC: {
      if (unary->right_->IsVariable()) {
        const std::string& name = std::static_pointer_cast<Literal>(unary->right_)->op_->GetStringValue();
        out_ << "\tsub" << GetPostfix(unary->right_->type_, unary->right_->indirection_)
             << "\t$" << (unary->to_scale_ ? type_sizes_[unary->right_->type_] : 1) << ", " << name << "(%rip)\n";
        out_ << "\tmov" << GetPostfix(unary->right_->type_, unary->right_->indirection_) << "\t"
             << name << "(%rip), " << GetRegister(r, unary->right_->type_, unary->right_->indirection_) << "\n";
      } else {
        out_ << "\tsub" << GetPostfix(unary->right_->type_, unary->right_->indirection_)
             << "\t$1, (" << kRegisters[r] << ")\n";
        out_ << "\tmov" << GetPostfix(unary->right_->type_, unary->right_->indirection_)
             << "\t(" << kRegisters[r] << "), "
             << GetRegister(r, unary->right_->type_, unary->right_->indirection_)
             << "\n";
      }
      break;
    }
    case TokenType::T_BIT_AND: {
      if (unary->right_->IsVariable()) {
        const std::string& name = std::static_pointer_cast<Literal>(unary->right_)->op_->GetStringValue();
        out_ << "\tleaq\t" << name << "(%rip), " << kRegisters[r] << "\n";
      }
      break;
    }
    case TokenType::T_STAR: {
      if (!unary->return_ptr_)
        out_ << "\tmovq\t" << "(" << kRegisters[r] << ")" << ", " << kRegisters[r] << "\n";
      break;
    }
    default: {
      std::cerr << "Invalid operator in binary expression: " << unary->op_ << std::endl;
      exit(1);
    }
  }

  return r;
}

int CodeGenX86::Visit(const std::shared_ptr<Binary>& binary) {
  int r1 = binary->left_->Accept(*this);
  int r2 = binary->right_->Accept(*this);

  if (binary->left_->to_scale_) {
    out_ << "\timulq\t$" << GetTypeSize(binary->right_->type_, 0) << ", " << kRegisters[r1] << "\n";
  }

  if (binary->right_->to_scale_) {
    out_ << "\timulq\t$" << GetTypeSize(binary->left_->type_, 0) << ", " << kRegisters[r2] << "\n";
  }

  switch (binary->op_->GetType()) {
    case TokenType::T_PLUS:
      out_ << "\taddq\t" << kRegisters[r1] << ", " << kRegisters[r2] << "\n";
      FreeRegister(r1);
      return r2;
    case TokenType::T_MINUS:
      out_ << "\tsubq\t" << kRegisters[r2] << ", " << kRegisters[r1] << "\n";
      FreeRegister(r2);
      return r1;
    case TokenType::T_STAR:
      out_ << "\timulq\t" << kRegisters[r1] << ", " << kRegisters[r2] << "\n";
      FreeRegister(r1);
      return r2;
    case TokenType::T_SLASH:
      out_ << "\tmovq\t" << kRegisters[r1] << ", %rax\n"
           << "\tcqo\n"
           << "\tidivq\t" << kRegisters[r2] << "\n"
           << "\tmovq\t%rax, " << kRegisters[r1] << "\n";
      FreeRegister(2);
      return r1;
    case TokenType::T_OR:
    case TokenType::T_AND:
      std::cerr << "Not implemented error '&&', '||' operators" << std::endl;
      exit(1);
    case TokenType::T_BIT_AND:
      out_ << "\tandq\t" << kRegisters[r1] << ", " << kRegisters[r2] << "\n";
      FreeRegister(r1);
      return r2;
    case TokenType::T_BIT_OR:
      out_ << "\torq\t" << kRegisters[r1] << ", " << kRegisters[r2] << "\n";
      FreeRegister(r1);
      return r2;
    case TokenType::T_BIT_XOR:
      out_ << "\txorq\t" << kRegisters[r1] << ", " << kRegisters[r2] << "\n";
      FreeRegister(r1);
      return r2;
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
      out_ << "\t" << GetSetInstr(binary->op_->GetType()) << "\t" << kBregisters[r2] << "\n";
      out_ << "\tmovzbq\t" << kBregisters[r2] << ", " << kRegisters[r2] << "\n";
      FreeRegister(r1);
      return r2;
    default:
      std::cerr << "Invalid operator in binary expression: " << binary->op_ << std::endl;
      exit(1);
  }
}

// #FIXME: Create separate AST node for identifier?
int CodeGenX86::Visit(const std::shared_ptr<Literal>& literal) {
  int r = NewRegister();

  // array
  if (literal->is_indexable_) {
    out_ << "\tleaq\t" << literal->op_->GetStringValue() << "(%rip), " << kRegisters[r] << "\n";
    return r;
  }

  // pointer
  if (literal->indirection_ != 0) {
    out_ << "\tmovq\t" << literal->op_->GetStringValue() << "(%rip), " << kRegisters[r] << "\n";
    return r;
  }

  // primitives
  switch (literal->op_->GetType()) {
    case TokenType::T_INT_LITERAL:
      out_ << "\tmovq\t$" << literal->op_->GetIntValue()
           << "," << kRegisters[r] << "\n";
      break;
    case TokenType::T_IDENTIFIER:
      switch (literal->type_) {
        case Type::SHORT:
          out_ << "\tmovzwq\t" << literal->op_->GetStringValue()
               << "(%rip), " << kRegisters[r] << "\n";
          break;
        case Type::INT:
          out_ << "\tmovslq\t" << literal->op_->GetStringValue()
               << "(%rip), " << kRegisters[r] << "\n";
          break;
        case Type::LONG:
          out_ << "\tmovq\t" << literal->op_->GetStringValue() << "(%rip), " << kRegisters[r] << "\n";
          break;
        default: {
          std::cerr << "Invalid literal type. Exiting." << std::endl;
          exit(1);
        }
      }
      break;
    default:
      std::cerr << "Invalid literal type '" << literal->op_->GetType()
        << "'." << std::endl;
      exit(1);
  }

  return r;
}

int CodeGenX86::Visit(const std::shared_ptr<Print>& print) {
  int r = print->expr_->Accept(*this);
  PrintInt(r);
  FreeRegister(r);
  return NO_RETURN_REGISTER;
}

// #FIXME: pass <Literal> as type, or cast <Expr> to here?
int CodeGenX86::Visit(const std::shared_ptr<Assign>& assign) {
  int r1 = assign->right_->Accept(*this);

  if (assign->left_->IsVariable()) {
    const std::string& name = std::static_pointer_cast<Literal>(assign->left_)->op_->GetStringValue();
    out_ << "\tmov" << GetPostfix(assign->left_->type_, assign->left_->indirection_) << "\t"
         << GetRegister(r1, assign->left_->type_, assign->left_->indirection_)
         << ", " << name << "(%rip)\n";
  } else {
    int r2 = assign->left_->Accept(*this);
    out_ << "\tmovq\t" << kRegisters[r1] << ", (" << kRegisters[r2] << ")\n";
    FreeRegister(r2);
  }

  return r1;
}

int CodeGenX86::Visit(const std::shared_ptr<DeclList>& decl_list) {
  for (const auto& var_decl : decl_list->var_decl_list_)
    var_decl->Accept(*this);
  return NO_RETURN_REGISTER;
}

int CodeGenX86::Visit(const std::shared_ptr<ExprList>& expr_list) {
  for (const auto& expr : expr_list->expr_list_)
    FreeRegister(expr->Accept(*this));
  return NO_RETURN_REGISTER;
}

int CodeGenX86::GetTypeSize(Type type, int indirection) {
  if (indirection > 0) return 8;
  return type_sizes_[type];
}

std::string CodeGenX86::GetPostfix(Type type, int ind) {
  if (ind > 0) return "q";
  switch (type) {
    case Type::SHORT: return "w";
    case Type::INT: return "l";
    case Type::LONG: return "q";
    default: {
      reporter_.Report("Invalid variable type.");
      exit(1);
    }
  }
}

std::string CodeGenX86::GetRegister(int r, Type type, int ind) {
  if (ind > 0) return kRegisters[r];
  switch (type) {
    case Type::SHORT: return kWregisters[r];
    case Type::INT: return kDregisters[r];
    case Type::LONG: return kRegisters[r];
    default: {
      reporter_.Report("Invalid variable type.");
      exit(1);
    }
  }
}

int CodeGenX86::Visit(const std::shared_ptr<VarDecl>& var_decl) {
  int size = GetTypeSize(var_decl->var_type_, var_decl->indirection_);
  const std::string& name = var_decl->name_->GetStringValue();

  if (var_decl->array_len_ > 0) {
    out_ << "\t.data\n" << "\t.globl\t" << name << "\n";
    out_ << name << ":\n";
    for (int i = 0; i < var_decl->array_len_; i++) {
      out_ << "\t." << GetAllocType(var_decl->var_type_, var_decl->indirection_) << "\t0\n";
    }
  } else {
    out_ << "\t.comm\t" << name << "," << size << "," << size << "\n";
    if (var_decl->init_ != nullptr) {
      int r = var_decl->init_->Accept(*this);
      out_ << "\tmov" << GetPostfix(var_decl->var_type_, var_decl->indirection_) << "\t"
           << GetRegister(r, var_decl->var_type_, var_decl->indirection_)
           << ", " << name << "(%rip)\n";
      FreeRegister(r);
    }
  }

  return NO_RETURN_REGISTER;
}

// #TODO: add local variables and parameter code generation
int CodeGenX86::Visit(const std::shared_ptr<FuncDecl>& func_decl) {
  out_ << "\t.text\n";
  out_ << "\t.globl\t" << func_decl->name_->GetStringValue() << "\n";
  out_ << "\t.type\t" << func_decl->name_->GetStringValue() << ", @function\n";
  out_ << func_decl->name_->GetStringValue() << ":\n";
  out_ << "\tpushq\t%rbp\n";
  out_ << "\tmovq\t%rsp, %rbp\n";
  return_label_ = GetLabel();
  func_decl->body_->Accept(*this);
  out_ << "L" << return_label_ << ":\n";
  out_ << "\tpopq\t%rbp\n";
  out_ << "\tret\n";
  return NO_RETURN_REGISTER;
}

int CodeGenX86::Visit(const std::shared_ptr<Return>& return_stmt) {
  int r = return_stmt->expr_->Accept(*this);
  out_ << "\tmovq\t" << kRegisters[r] << ", %rax\n";
  out_ << "\tjmp\tL" << return_label_ << "\n";
  FreeRegister(r);
  return NO_RETURN_REGISTER;
}

int CodeGenX86::Visit(const std::shared_ptr<ExpressionStmt>& expr_stmt) {
  int r = expr_stmt->expr_->Accept(*this);
  FreeRegister(r);
  return NO_RETURN_REGISTER;
}

int CodeGenX86::Visit(const std::shared_ptr<Grouping>& grouping) {
  return grouping->expr_->Accept(*this);
}

// array[2] = 15;
// print array[2];
int CodeGenX86::Visit(const std::shared_ptr<Index>& index) {
  int r1 = index->name_->Accept(*this);
  int r2 = index->index_->Accept(*this);

  // scale the index
  out_ << "\timulq\t$" << type_sizes_[index->name_->type_] << ", " << kRegisters[r2] << "\n";
  out_ << "\taddq\t" << kRegisters[r2] << ", " << kRegisters[r1] << "\n";
  FreeRegister(r2);

  if (!index->return_ptr_) {
    out_ << "\tmovq\t(" << kRegisters[r1] << "), " << kRegisters[r1] << "\n";
  }

  return r1;
}

int CodeGenX86::Visit(const std::shared_ptr<Call>& call) {
  out_ << "\tcall\t" << std::static_pointer_cast<Literal>(call->name_)->op_->GetStringValue() << "\n";

  if (call->type_ != Type::VOID) {
    int r = NewRegister();
    out_ << "\tmovq\t%rax, " << kRegisters[r] << "\n";
    return r;
  }

  return NO_RETURN_REGISTER;
}

int CodeGenX86::Visit(const std::shared_ptr<ControlFlow>& flow_stmt) {
  if (loop_stack_.empty()) {
    std::cerr << ((flow_stmt->is_break_) ? "break" : "continue") << " does not have enclosing loop." << std::endl;
    exit(1);
  }

  const auto& labels = loop_stack_.top();
  out_ << "\tjmp\t" << (flow_stmt->is_break_ ? labels.first : labels.second) << "\n";
  return NO_RETURN_REGISTER;
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

void CodeGenX86::Preamble() {
  out_ <<
      "\t.text\n"
      ".LC0:\n"
      "\t.string\t\"%d\\n\"\n"
      "printint:\n"
      "\tpushq\t%rbp\n"
      "\tmovq\t%rsp, %rbp\n"
      "\tsubq\t$16, %rsp\n"
      "\tmovl\t%edi, -4(%rbp)\n"
      "\tmovl\t-4(%rbp), %eax\n"
      "\tmovl\t%eax, %esi\n"
      "\tleaq	.LC0(%rip), %rdi\n"
      "\tmovl	$0, %eax\n"
      "\tcall	printf@PLT\n"
      "\tnop\n"
      "\tleave\n"
      "\tret\n";
}

void CodeGenX86::Postamble() { }
void CodeGenX86::Generate(const std::vector<std::shared_ptr<Stmt>>& stmts) {
  Preamble();
  for (const auto& stmt : stmts)
    stmt->Accept(*this);
  Postamble();
}
void CodeGenX86::PrintInt(int r) {
  out_ << "\tmovq\t" << kRegisters[r] << ", %rdi\n"
    << "\tcall\tprintint\n";
}
int CodeGenX86::NewRegister() {
  for (int i = 0; i < REGISTER_NUM; i++) {
    if (regs_status[i]) {
      regs_status[i] = false;
      return i;
    }
  }
  std::cerr << "Run out of registers." << std::endl; // #TODO: Register spilling
  exit(1);
}
void CodeGenX86::FreeRegister(int reg) {
  if (reg == NO_RETURN_REGISTER)
    return;
  regs_status[reg] = true;
}

int CodeGenX86::GetLabel() {
  return label_++;
}

int CodeGenX86::Visit(const std::shared_ptr<Ternary>& ternary) {
  int r = ternary->condition_->Accept(*this);
  int l_start = GetLabel(), l_end = GetLabel();
  out_ << "\tcmpq\t$0, " << kRegisters[r] << "\n";

  out_ << "\tje\t" << "L" << l_start << "\n";
  int r1 = ternary->then_->Accept(*this);
  out_ << "\tmovq\t" << kRegisters[r1] << ", " << kRegisters[r] << "\n";
  FreeRegister(r1);

  out_ << "\tjmp\t" << "L" << l_end << "\n";
  out_ << "L" << l_start << ":\n";
  int r2 = ternary->else_->Accept(*this);
  out_ << "\tmovq\t" << kRegisters[r2] << ", " << kRegisters[r] << "\n";
  FreeRegister(r2);

  out_ << "L" << l_end << ":\n";
  return r;
}

int CodeGenX86::Visit(const std::shared_ptr<Postfix>& postfix) {
  int r1 = postfix->expr_->Accept(*this);

  switch (postfix->op_->GetType()) {
    case TokenType::T_INC: {
      if (postfix->expr_->IsVariable()) {
        const std::string& name = std::static_pointer_cast<Literal>(postfix->expr_)->op_->GetStringValue();
        out_ << "\tadd" << GetPostfix(postfix->expr_->type_, postfix->expr_->indirection_)
             << "\t$" << (postfix->to_scale_ ? type_sizes_[postfix->expr_->type_] : 1)
             << ", " << name << "(%rip)\n";
        break;
      } else {
        int r2 = NewRegister();
        out_ << "\tmov" << GetPostfix(postfix->expr_->type_, postfix->expr_->indirection_)
             << "\t(" << kRegisters[r1] << "), "
             << GetRegister(r2, postfix->expr_->type_, postfix->expr_->indirection_)
             << "\n";
        out_ << "\tadd"
             << GetPostfix(postfix->expr_->type_, postfix->expr_->indirection_)
             << "\t$1, (" << kRegisters[r1] << ")\n";
        FreeRegister(r1);
        return r2;
      }
    }
    case TokenType::T_DEC: {
      if (postfix->expr_->IsVariable()) {
        const std::string& name = std::static_pointer_cast<Literal>(postfix->expr_)->op_->GetStringValue();
        out_ << "\tsub" << GetPostfix(postfix->expr_->type_, postfix->expr_->indirection_)
             << "\t$" << (postfix->to_scale_ ? type_sizes_[postfix->expr_->type_] : 1)
             << ", " << name << "(%rip)\n";
        break;
      } else {
        int r2 = NewRegister();
        out_ << "\tmov" << GetPostfix(postfix->expr_->type_, postfix->expr_->indirection_)
             << "\t(" << kRegisters[r1] << "), "
             << GetRegister(r2, postfix->expr_->type_, postfix->expr_->indirection_)
             << "\n";
        out_ << "\tsub"
             << GetPostfix(postfix->expr_->type_, postfix->expr_->indirection_)
             << "\t$1, (" << kRegisters[r1] << ")\n";
        FreeRegister(r1);
        return r2;
      }
    }
    default: {
      std::cerr << "Invalid operator in postfix expression." << std::endl;
      exit(1);
    }
  }

  return r1;
}

std::string CodeGenX86::GetAllocType(Type type, int ind) {
  if (ind > 0) return "quad";
  switch (type) {
    case Type::SHORT: return "word";
    case Type::INT: return "long";
    case Type::LONG: return "quad";
    default: {
      std::cerr << "Invalid type in array allocation." << type << std::endl;
      exit(1);
    }
  }
}

} // namespace mcc
