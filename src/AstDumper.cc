//
// Created by vladyslav on 01.02.20.
//

#include "AstDumper.h"

namespace mcc {

// #TODO: rewrite 'spaces += TAB_SIZE' thing

int AstDumper::Visit(const std::shared_ptr<Binary>& binary) {
  spaces_ += TAB_SIZE;
  out_ << std::string(spaces_, ' ') << "<binary op=" << binary->op_->GetType() << ">" << "\n";
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
  if (literal->IsVariable())
    out_ << literal->op_->GetStringValue();
  else
    out_ << literal->op_->GetIntValue();
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
  out_ << std::string(spaces_ + 2, ' ') << "<name>" << var_decl->name_->GetStringValue() << "</name>\n";
  out_ << std::string(spaces_ + 2, ' ') << "<ind>" << var_decl->indirection_ << "</ind>\n";

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

int AstDumper::Visit(const std::shared_ptr<Grouping>& grouping) {
  spaces_ += TAB_SIZE;
  out_ << std::string(spaces_, ' ') << "<grouping>\n";
  grouping->expr_->Accept(*this);
  out_ << std::string(spaces_, ' ') << "</grouping>\n";
  spaces_ -= TAB_SIZE;
  return 0;
}

int AstDumper::Visit(const std::shared_ptr<ExpressionStmt>& expr_stmt) {
  spaces_ += TAB_SIZE;
  out_ << std::string(spaces_, ' ') << "<expr-stmt>\n";
  expr_stmt->expr_->Accept(*this);
  out_ << std::string(spaces_, ' ') << "</expr-stmt>\n";
  spaces_ -= TAB_SIZE;
  return 0;
}

int AstDumper::Visit(const std::shared_ptr<Ternary> &ternary) {
  spaces_ += TAB_SIZE;
  out_ << std::string(spaces_, ' ') << "<ternary>\n";

  out_ << std::string(spaces_ + 2, ' ') << "<condition>\n";
  spaces_ += TAB_SIZE;
  ternary->condition_->Accept(*this);
  spaces_ -= TAB_SIZE;
  out_ << std::string(spaces_ + 2, ' ') << "</condition>\n";

  out_ << std::string(spaces_ + 2, ' ') << "<then>\n";
  spaces_ += TAB_SIZE;
  ternary->then_->Accept(*this);
  spaces_ -= TAB_SIZE;
  out_ << std::string(spaces_ + 2, ' ') << "</then>\n";

  out_ << std::string(spaces_ + 2, ' ') << "<else>\n";
  spaces_ += TAB_SIZE;
  ternary->else_->Accept(*this);
  spaces_ -= TAB_SIZE;
  out_ << std::string(spaces_ + 2, ' ') << "</else>\n";

  out_ << std::string(spaces_, ' ') << "</ternary>" << std::endl;
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
  cond_stmt->then_block_->Accept(*this);
  spaces_ -= TAB_SIZE;
  out_ << std::string(spaces_ + 2, ' ') << "</if>\n";

  if (cond_stmt->else_block_ != nullptr) {
    out_ << std::string(spaces_ + 2, ' ') << "<else>\n";
    spaces_ += TAB_SIZE;
    cond_stmt->else_block_->Accept(*this);
    spaces_ -= TAB_SIZE;
    out_ << std::string(spaces_ + 2, ' ') << "</else>\n";
  }

  out_ << std::string(spaces_, ' ') << "</if-stmt>" << std::endl;
  spaces_ -= TAB_SIZE;
  return 0;
}

int AstDumper::Visit(const std::shared_ptr<Call>& call) {
  spaces_ += TAB_SIZE;
  out_ << std::string(spaces_, ' ') << "<call>" << std::endl;
  out_ << std::string(spaces_ + 2, ' ') << "<name>\n";
  call->name_->Accept(*this);
  out_ << std::string(spaces_ + 2, ' ') << "</name>\n";

  out_ << std::string(spaces_ + 2, ' ') << "<args>\n";
  spaces_ += TAB_SIZE;
  call->args_->Accept(*this);
  spaces_ -= TAB_SIZE;
  out_ << std::string(spaces_ + 2, ' ') << "</args>\n";

  out_ << std::string(spaces_, ' ') << "</call>" << std::endl;
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
  while_stmt->loop_block_->Accept(*this);
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

  if (for_stmt->condition_ != nullptr) {
    out_ << std::string(spaces_ + 2, ' ') << "<condition>\n";
    spaces_ += TAB_SIZE;
    for_stmt->condition_->Accept(*this);
    spaces_ -= TAB_SIZE;
    out_ << std::string(spaces_ + 2, ' ') << "</condition>\n";
  }

  out_ << std::string(spaces_ + 2, ' ') << "<loop>\n";
  spaces_ += TAB_SIZE;
  for_stmt->loop_block_->Accept(*this);
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

int AstDumper::Visit(const std::shared_ptr<ControlFlow>& flow_stmt) {
  spaces_ += TAB_SIZE;
  if (flow_stmt->is_break_) {
    out_ << std::string(spaces_, ' ') << "<break/>\n";
  } else {
    out_ << std::string(spaces_, ' ') << "<continue/>\n";
  }
  spaces_ -= TAB_SIZE;
  return 0;
}

int AstDumper::Visit(const std::shared_ptr<Postfix>& postfix) {
  spaces_ += TAB_SIZE;
  out_ << std::string(spaces_, ' ') << "<postfix op='" << postfix->op_->GetType() << "'>" << "\n";
  postfix->expr_->Accept(*this);
  out_ << std::string(spaces_, ' ') << "</postfix>" << "\n";
  spaces_ -= TAB_SIZE;
  return 0;
}

int AstDumper::Visit(const std::shared_ptr<Unary>& unary) {
  spaces_ += TAB_SIZE;
  out_ << std::string(spaces_, ' ') << "<unary op='" << unary->op_->GetType() << "'>" << "\n";
  unary->expr_->Accept(*this);
  out_ << std::string(spaces_, ' ') << "</unary>" << "\n";
  spaces_ -= TAB_SIZE;
  return 0;
}

// #TODO: add params
int AstDumper::Visit(const std::shared_ptr<FuncDecl>& func_decl) {
  spaces_ += TAB_SIZE;
  out_ << std::string(spaces_, ' ') << "<func-decl>\n";

  out_ << std::string(spaces_ + 2, ' ') << "<name>" << func_decl->name_->GetStringValue() << "</name>\n";
  out_ << std::string(spaces_ + 2, ' ') << "<type>" << func_decl->return_type_ << "</type>\n";
  out_ << std::string(spaces_ + 2, ' ') << "<ind>" << func_decl->indirection_ << "</ind>\n";

  out_ << std::string(spaces_ + 2, ' ') << "<body>\n";
  spaces_ += TAB_SIZE;
  func_decl->body_->Accept(*this);
  spaces_ -= TAB_SIZE;
  out_ << std::string(spaces_ + 2, ' ') << "</body>\n";

  out_ << std::string(spaces_, ' ') << "</func-decl>\n";
  spaces_ -= TAB_SIZE;
  return 0;
}
int AstDumper::Visit(const std::shared_ptr<Return>& return_stmt) {
  spaces_ += TAB_SIZE;
  out_ << std::string(spaces_, ' ') << "<return>\n";
  return_stmt->expr_->Accept(*this);
  out_ << std::string(spaces_, ' ') << "</return>\n";
  spaces_ -= TAB_SIZE;
  return 0;
}

int AstDumper::Visit(const std::shared_ptr<Index>& index) {
  spaces_ += TAB_SIZE;
  out_ << std::string(spaces_, ' ') << "<index>" << std::endl;
  out_ << std::string(spaces_ + 2, ' ') << "<name>\n";
  index->name_->Accept(*this);
  out_ << std::string(spaces_ + 2, ' ') << "</name>\n";

  out_ << std::string(spaces_ + 2, ' ') << "<index-expr>\n";
  spaces_ += TAB_SIZE;
  index->index_->Accept(*this);
  spaces_ -= TAB_SIZE;
  out_ << std::string(spaces_ + 2, ' ') << "</index-expr>\n";

  out_ << std::string(spaces_, ' ') << "</index>" << std::endl;
  spaces_ -= TAB_SIZE;
  return 0;
}

} // namespace mcc
