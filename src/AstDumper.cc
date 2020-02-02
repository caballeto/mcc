//
// Created by vladyslav on 01.02.20.
//

#include "AstDumper.h"

namespace mcc {

// #TODO: rewrite 'spaces += TAB_SIZE' thing

int AstDumper::Visit(const std::shared_ptr<Binary>& binary) {
  spaces_ += TAB_SIZE;
  out_ << std::string(spaces_, ' ') << "<binary op='" << binary->op_->GetType() << "'>" << "\n";
  binary->left_->Accept(*this);
  binary->right_->Accept(*this);
  out_ << std::string(spaces_, ' ') << "</binary>" << "\n";
  spaces_ -= TAB_SIZE;
  return 0;
}

int AstDumper::Visit(const std::shared_ptr<Print>& print) {
  spaces_ += TAB_SIZE;
  out_ << std::string(spaces_, ' ') << "<stmt type='print'>\n";
  print->expr_->Accept(*this);
  out_ << std::string(spaces_, ' ') << "</stmt>\n";
  spaces_ -= TAB_SIZE;
  return 0;
}

int AstDumper::Visit(const std::shared_ptr<Literal>& literal) {
  spaces_ += TAB_SIZE;
  out_ << std::string(spaces_, ' ')
     << "<literal val='";
  if (!literal->literal_->GetStringValue().empty())
    out_ << literal->literal_->GetStringValue();
  else
    out_ << literal->literal_->GetIntValue();
  out_ << "'></literal>" << "\n";
  spaces_ -= TAB_SIZE;
  return 0;
}

int AstDumper::Visit(const std::shared_ptr<Assign>& assign) {
  spaces_ += TAB_SIZE;
  out_ << std::string(spaces_, ' ') << "<assign>\n";
  assign->left_->Accept(*this);
  assign->right_->Accept(*this);
  out_ << std::string(spaces_, ' ') << "</assign>\n";
  spaces_ -= TAB_SIZE;
  return 0;
}

int AstDumper::Visit(const std::shared_ptr<VarDecl>& var_decl) {
  spaces_ += TAB_SIZE;
  out_ << std::string(spaces_, ' ') << "<var-decl>\n";
  out_ << std::string(spaces_ + 2, ' ') << "<name>" << var_decl->name_ << "</name>\n";

  if (var_decl->init_ != nullptr) {
    out_ << std::string(spaces_ + 2, ' ') << "<init>\n";
    spaces_ += TAB_SIZE;
    var_decl->init_->Accept(*this);
    spaces_ -= TAB_SIZE;
    out_ << std::string(spaces_ + 2, ' ') << "</init>\n";
  }

  out_ << std::string(spaces_, ' ') << "</var-decl>\n";
  spaces_ -= TAB_SIZE;
  return 0;
}

int AstDumper::Visit(const std::shared_ptr<ExpressionStmt>& expr_stmt) {
  spaces_ += TAB_SIZE;
  out_ << std::string(spaces_, ' ') << "<expr-stmt>\n";
  expr_stmt->expr_->Accept(*this);
  out_ << std::string(spaces_, ' ') << "</expr-stmt>" << std::endl;
  spaces_ -= TAB_SIZE;
  return 0;
}

int AstDumper::Visit(const std::shared_ptr<Conditional>& cond_stmt) {
  spaces_ += TAB_SIZE;
  out_ << std::string(spaces_, ' ') << "<if-stmt>\n";

  out_ << std::string(spaces_ + 2, ' ') << "<condition>\n";
  spaces_ += TAB_SIZE;
  cond_stmt->condition_->Accept(*this);
  spaces_ -= TAB_SIZE;
  out_ << std::string(spaces_ + 2, ' ') << "</condition>\n";

  out_ << std::string(spaces_ + 2, ' ') << "<if>\n";
  spaces_ += TAB_SIZE;
  Dump(cond_stmt->then_block_->stmts_);
  spaces_ -= TAB_SIZE;
  out_ << std::string(spaces_ + 2, ' ') << "</if>\n";

  if (!cond_stmt->then_block_->stmts_.empty()) {
    out_ << std::string(spaces_ + 2, ' ') << "<else>\n";
    spaces_ += TAB_SIZE;
    Dump(cond_stmt->else_block_->stmts_);
    spaces_ -= TAB_SIZE;
    out_ << std::string(spaces_ + 2, ' ') << "</else>\n";
  }

  out_ << std::string(spaces_, ' ') << "</if-stmt>" << std::endl;
  spaces_ -= TAB_SIZE;
  return 0;
}

int AstDumper::Visit(const std::shared_ptr<Block>& block_stmt) {
  spaces_ += TAB_SIZE;
  out_ << std::string(spaces_, ' ') << "<block>" << std::endl;
  Dump(block_stmt->stmts_);
  out_ << std::string(spaces_, ' ') << "</block>" << std::endl;
  spaces_ -= TAB_SIZE;
  return 0;
}

void AstDumper::Dump(const std::vector<std::shared_ptr<Stmt>>& stmts) {
  for (const auto& stmt : stmts) {
    stmt->Accept(*this);
  }
}

void AstDumper::Flush() {
  out_ << std::flush;
}

int AstDumper::Visit(const std::shared_ptr<While>& while_stmt) {
  spaces_ += TAB_SIZE;
  out_ << std::string(spaces_, ' ') << "<while-stmt type='";
  out_ << (while_stmt->do_while_ ? "do-while" : "while") << "'>\n";

  out_ << std::string(spaces_ + 2, ' ') << "<condition>\n";
  spaces_ += TAB_SIZE;
  while_stmt->condition_->Accept(*this);
  spaces_ -= TAB_SIZE;
  out_ << std::string(spaces_ + 2, ' ') << "</condition>\n";

  out_ << std::string(spaces_ + 2, ' ') << "<loop>\n";
  spaces_ += TAB_SIZE;
  Dump(while_stmt->loop_block_->stmts_);
  spaces_ -= TAB_SIZE;
  out_ << std::string(spaces_ + 2, ' ') << "</loop>\n";

  out_ << std::string(spaces_, ' ') << "</while-stmt>" << std::endl;
  spaces_ -= TAB_SIZE;
  return 0;
}

int AstDumper::Visit(const std::shared_ptr<For>& for_stmt) {
  spaces_ += TAB_SIZE;
  out_ << std::string(spaces_, ' ') << "<for-stmt>\n";

  out_ << std::string(spaces_ + 2, ' ') << "<init>\n";
  spaces_ += TAB_SIZE;
  for_stmt->init_->Accept(*this);
  spaces_ -= TAB_SIZE;
  out_ << std::string(spaces_ + 2, ' ') << "</init>\n";

  out_ << std::string(spaces_ + 2, ' ') << "<condition>\n";
  spaces_ += TAB_SIZE;
  for_stmt->condition_->Accept(*this);
  spaces_ -= TAB_SIZE;
  out_ << std::string(spaces_ + 2, ' ') << "</condition>\n";

  out_ << std::string(spaces_ + 2, ' ') << "<loop>\n";
  spaces_ += TAB_SIZE;
  Dump(for_stmt->loop_block_->stmts_);
  spaces_ -= TAB_SIZE;
  out_ << std::string(spaces_ + 2, ' ') << "</loop>\n";

  out_ << std::string(spaces_ + 2, ' ') << "<update>\n";
  spaces_ += TAB_SIZE;
  for_stmt->update_->Accept(*this);
  spaces_ -= TAB_SIZE;
  out_ << std::string(spaces_ + 2, ' ') << "</update>\n";

  out_ << std::string(spaces_, ' ') << "</for-stmt>" << std::endl;
  spaces_ -= TAB_SIZE;
  return 0;
}

int AstDumper::Visit(const std::shared_ptr<DeclList>& decl_list) {
  spaces_ += TAB_SIZE;
  out_ << std::string(spaces_, ' ') << "<decl-list>\n";

  for (const auto& decl : decl_list->var_decl_list_) {
    decl->Accept(*this);
  }

  out_ << std::string(spaces_, ' ') << "</decl-list>\n";
  spaces_ -= TAB_SIZE;
  return 0;
}

int AstDumper::Visit(const std::shared_ptr<ExprList>& expr_list) {
  spaces_ += TAB_SIZE;
  out_ << std::string(spaces_, ' ') << "<expr-list>\n";

  for (const auto& expr : expr_list->expr_list_) {
    expr->Accept(*this);
  }

  out_ << std::string(spaces_, ' ') << "</expr-list>\n";
  spaces_ -= TAB_SIZE;
  return 0;
}

} // namespace mcc
