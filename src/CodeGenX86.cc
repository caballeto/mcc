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

int CodeGenX86::Visit(const std::shared_ptr<Binary>& binary) {
  int r1 = binary->left_->Accept(*this);
  int r2 = binary->right_->Accept(*this);
  // b ->
  // add w -> 16
  // l -> 32
  // q -> 64
  // GetPostfix(Type) -> {b, "", l, q}
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
    case TokenType::T_EQUALS:
    case TokenType::T_NOT_EQUALS:
    case TokenType::T_GREATER:
    case TokenType::T_LESS:
    case TokenType::T_GREATER_EQUAL:
    case TokenType::T_LESS_EQUAL:
      out_ << "\tcmpq\t" << kRegisters[r2] << ", " << kRegisters[r1] << "\n";
      out_ << "\t" << GetSetInstr(binary->op_->GetType()) << "\t" << kBregisters[r2] << "\n";
      out_ << "\tandq\t$255, " << kRegisters[r2] << "\n";
      FreeRegister(r1);
      return r2;
    default:
      std::cerr << "Invalid operator in binary expression: " << binary->op_ << std::endl;
      exit(1);
  }
}

// #FIXME: Create separate AST node for identifier?
int CodeGenX86::Visit(const std::shared_ptr<Literal>& literal) {
  int reg = NewRegister();
  switch (literal->op_->GetType()) {
    case TokenType::T_INT_LITERAL:
      out_ << "\tmovq\t$" << literal->op_->GetIntValue()
           << "," << kRegisters[reg] << "\n";
      break;
    case TokenType::T_IDENTIFIER:
      out_ << "\tmovq\t" << literal->op_->GetStringValue()
           << "(%rip), " << kRegisters[reg] << "\n";
      break;
    default:
      std::cerr << "Invalid literal type '" << literal->op_->GetType()
        << "'." << std::endl;
      exit(1);
  }

  return reg;
}

int CodeGenX86::Visit(const std::shared_ptr<Print>& print) {
  int r = print->expr_->Accept(*this);
  PrintInt(r);
  FreeRegister(r);
  return NO_RETURN_REGISTER;
}

// #FIXME: pass <Literal> as type, or cast <Expr> to here?
int CodeGenX86::Visit(const std::shared_ptr<Assign>& assign) {
  int r = assign->right_->Accept(*this);
  if (assign->left_->IsVariable()) {
    out_ << "\tmovq\t" << kRegisters[r] << ", "
         << std::static_pointer_cast<Literal>(assign->left_)->op_->GetStringValue()
         << "(%rip)\n";
  } else {
    std::cerr << "Assign to non variable in assign expression." << std::endl;
    exit(1);
  }
  return r;
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

int CodeGenX86::GetTypeSize(Type type) {
  switch (type) {
    case Type::SHORT: return 2;
    case Type::INT: return 4;
    case Type::LONG: return 8;
    default: {
      reporter_.Report("Invalid variable type.");
      exit(1);
    }
  }
}

std::string CodeGenX86::GetPostfix(Type type) {
  switch (type) {
    case Type::SHORT: return "";
    case Type::INT: return "l";
    case Type::LONG: return "q";
    default: {
      reporter_.Report("Invalid variable type.");
      exit(1);
    }
  }
}

std::string CodeGenX86::GetRegister(int r, Type type) {
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
  // int size = GetTypeSize(var_decl->var_type_);
  out_ << "\t.comm\t" << var_decl->name_->GetStringValue() << ",8,8\n";

  if (var_decl->init_ != nullptr) {
    int r = var_decl->init_->Accept(*this);
    out_ << "\tmovq\t" << kRegisters[r] << ", " << var_decl->name_->GetStringValue() << "(%rip)\n";
    FreeRegister(r);
  }

  return NO_RETURN_REGISTER;
}

int CodeGenX86::Visit(const std::shared_ptr<ExpressionStmt>& expr_stmt) {
  int r = expr_stmt->expr_->Accept(*this);
  FreeRegister(r);
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
      "\tret\n"
      "\n"
      "\t.globl\tmain\n"
      "\t.type\tmain, @function\n"
      "main:\n"
      "\tpushq\t%rbp\n"
      "\tmovq	%rsp, %rbp\n";
}

void CodeGenX86::Postamble() {
  out_ << "\tmovl	$0, %eax\n"
          "\tpopq	%rbp\n"
          "\tret\n";
}
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

} // namespace mcc
