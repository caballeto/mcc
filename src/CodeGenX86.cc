//
// Created by vladyslav on 15.01.20.
//

#include "CodeGenX86.h"

namespace mcc {

int CodeGenX86::Visit(const std::shared_ptr<Binary>& binary) {
  int r1 = binary->left_->Accept(*this);
  int r2 = binary->right_->Accept(*this);

  switch (binary->op_->GetType()) {
    case TokenType::T_PLUS:
      out_ << "\taddq\t" << rregisters[r1] << ", " << rregisters[r2] << "\n";
      FreeRegister(r1);
      return r2;
    case TokenType::T_MINUS:
      out_ << "\tsubq\t" << rregisters[r2] << ", " << rregisters[r1] << "\n";
      FreeRegister(r2);
      return r1;
    case TokenType::T_STAR:
      out_ << "\timulq\t" << rregisters[r1] << ", " << rregisters[r2] << "\n";
      FreeRegister(r1);
      return r2;
    case TokenType::T_SLASH:
      out_ << "\tmovq\t" << rregisters[r1] << ", %rax\n"
        << "\tcqo\n"
        << "\tidivq\t" << rregisters[r2] << "\n"
        << "\tmovq\t%rax, " << rregisters[r1] << "\n";
      FreeRegister(2);
      return r1;
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
        << "," << rregisters[reg] << "\n";
      break;
    case TokenType::T_IDENTIFIER:
      out_ << "\tmovq\t" << literal->literal_->GetStringValue()
        << "(%rip), " << rregisters[reg] << "\n";
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

int CodeGenX86::Visit(const std::shared_ptr<Assign>& assign) {
  int r = assign->right_->Accept(*this);
  out_ << "\tmovq\t" << rregisters[r] << ", " << assign->left_->literal_->GetStringValue()
    << "(%rip)\n";
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
  out_ << "\tmovq\t" << rregisters[r] << ", %rdi\n"
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

} // namespace mcc
