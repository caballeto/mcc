//
// Created by vladyslav on 15.01.20.
//

#include "CodeGenX86.h"

namespace mcc {

// #TODO: optimize for no 'else' case (don't generate end label)
int CodeGenX86::Visit(const std::shared_ptr<Conditional>& cond_stmt) {
  int r = cond_stmt->condition_->Accept(*this);
  int l_start = GetLabel(), l_end = GetLabel();
  FreeRegister(r);
  out_ << "\tcmpq\t$0, " << kRegisters[r] << "\n";
  out_ << "\tje\t" << "L" << l_start << "\n";
  cond_stmt->then_block_->Accept(*this);
  out_ << "\tjmp\t" << "L" << l_end << "\n";
  out_ << "L" << l_start << ":\n";
  cond_stmt->else_block_->Accept(*this);
  out_ << "L" << l_end << ":\n";
  return 0;
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
  switch (literal->literal_->GetType()) {
    case TokenType::T_INT_LITERAL:
      out_ << "\tmovq\t$" << literal->literal_->GetIntValue()
           << "," << kRegisters[reg] << "\n";
      break;
    case TokenType::T_IDENTIFIER:
      out_ << "\tmovq\t" << literal->literal_->GetStringValue()
           << "(%rip), " << kRegisters[reg] << "\n";
      break;
    default:
      std::cerr << "Invalid literal type '" << literal->literal_->GetType()
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
         << std::static_pointer_cast<Literal>(assign->left_)->literal_->GetStringValue()
         << "(%rip)\n";
  } else {
    std::cerr << "Assign to non variable in assign expression." << std::endl;
    exit(1);
  }
  return r;
}

int CodeGenX86::Visit(const std::shared_ptr<VarDecl>& var_decl) {
  out_ << "\t.comm\t" << var_decl->name_ << ",8,8\n";
  return NO_RETURN_REGISTER;
}

int CodeGenX86::Visit(const std::shared_ptr<ExpressionStmt>& expr_stmt) {
  int r = expr_stmt->expr_->Accept(*this);
  FreeRegister(r);
  return NO_RETURN_REGISTER;
}

std::string CodeGenX86::GetSetInstr(TokenType& type) {
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
  regs_status[reg] = true;
}

int CodeGenX86::GetLabel() {
  return label_++;
}

} // namespace mcc
