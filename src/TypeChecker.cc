//
// Created by vladyslav on 03.02.20.
//

#include "ErrorReporter.h"
#include "TypeChecker.h"
#include "Visitor.h"

namespace mcc {

// Rewrite ErrorReporter to support different error reporting
// Implement type checking for new types (short, int, long, void)
// Implement code generation for new types

// for tomorrow : big refactoring
// parser synchronization on errors
// error reporter to print tokens, line number, character counts
// support for pointers of different types
// testing routines + tests with gtest

bool TypeChecker::IsPointer(ExprRef expr) {
  if (expr == nullptr) return false;
  return expr->type_.IsPointer();
}

bool TypeChecker::IsIntegerType(ExprRef expr) {
  if (expr == nullptr) return false;
  return expr->type_.IsPrimitive() && !expr->type_.IsVoid();
}

// #TODO: assign of structs
bool TypeChecker::MatchTypeInit(const Type& type, ExprRef init) {
  if (init->type_.type_ == TokenType::T_NONE) return false;
  if (!type.IsPointer()) {
    return IsPointer(init) ? true : (int) type.type_ >= (int) init->type_.type_;
  } else {
    if (type.type_ != init->type()|| type.ind != init->ind())
      reporter_.Warning("Assign of different type to pointer without a cast", type, init, init->op_);
    return true;
  }
}

// #TODO: build type hierarchy as a separate structure and provide API for type casting
// assuming types are not void
bool TypeChecker::MatchTypes(ExprRef e1, ExprRef e2, bool to_left, ExprRef expr) {
  if (e1->type_.IsVoid() || e2->type_.IsVoid()) {
    reporter_.Error("Invalid operands types in binary expression, 'void' is not allowed",
                    e1, e2, expr->op_);
    return false;
  }

  if (IsPointer(e1) && IsPointer(e2)) {
    return MatchPointers(e1, e2, to_left, expr);
  } else if (!IsPointer(e1) && !IsPointer(e2)) {
    return MatchPrimitives(e1, e2, to_left, expr);
  } else {
    return MatchMixed(e1, e2, to_left, expr);
  }
}

bool TypeChecker::MatchPointers(ExprRef e1, ExprRef e2, bool to_left, ExprRef binary) {
  if (to_left && binary->op_->GetType() == TokenType::T_ASSIGN) {
    // FIXME: issue warnings on incompatible pointer casts
    binary->type_ = e1->type_;
    return true;
  }

  if (e1->type() != e2->type() || e1->ind() != e2->ind()) {
    reporter_.Error("Invalid operands types in binary expression", e1, e2, binary->op_);
  } else if (binary->op_->GetType() != TokenType::T_MINUS) {
    reporter_.Error("Invalid binary operation for types", e1, e2, binary->op_);
  } else {
    binary->type_.type_ = TokenType::T_LONG; // pointer subtraction yields long
    binary->type_.ind = 0;
    return true;
  }

  return false;
}

bool TypeChecker::MatchPrimitives(ExprRef e1, ExprRef e2, bool to_left, ExprRef binary) {
  if (e1->type() == e2->type()) {
    binary->type_ = e1->type_;
    return true;
  }

  // FIXME: fix for structs
  if (!IsIntegerType(e1) || !IsIntegerType(e2)) {
    reporter_.Error("Invalid operands types in binary expression", e1, e2, binary->op_);
    return false;
  }

  binary->type_ = to_left ? PromoteToLeft(e1, e2) : PromotePrim(e1, e2);
  return true;
}

bool TypeChecker::MatchMixed(ExprRef e1, ExprRef e2, bool to_left, ExprRef binary) {
  if (binary->op_->GetType() == TokenType::T_ASSIGN) { // assign
    binary->type_ = e1->type_;
    reporter_.Warning("Assignment with pointer/integer without a cast", e1, e2, binary->op_);
  } else if (IsComparison(binary->op_->GetType())) { // comparison
    binary->type_.type_ = TokenType::T_INT;
    binary->type_.ind = 0;
    reporter_.Warning("Comparison between pointer and integer", e1, e2, binary->op_);
  } else if (binary->op_->GetType() != TokenType::T_PLUS && binary->op_->GetType() != TokenType::T_MINUS) {
    reporter_.Error("Invalid operand for pointer arithmetic, only '+' and '-' allowed", e1, e2, binary->op_);
    return false;
  } else if (IsIntegerType(e1) && IsPointer(e2)) { // +, - for (int + pointer)
    if (binary->op_->GetType() == TokenType::T_MINUS) { // #TODO: check if can (x - ptr)
      reporter_.Error("Invalid operand for pointer arithmetic, only '+' allowed (int + ptr)", e1, e2, binary->op_);
      return false;
    }

    binary->type_ = e2->type_;
    e1->to_scale_ = true;
  } else if (IsIntegerType(e2) && IsPointer(e1)) { // +, - for (pointer + int)
    binary->type_ = e1->type_;
    e2->to_scale_ = true;
  } else {
    reporter_.Error("Invalid types for pointer arithmetic in binary expression", e1, e2, binary->op_);
    return false;
  }

  return true;
}

Type& TypeChecker::PromoteToLeft(ExprRef e1, ExprRef e2) {
  int t1 = (int) e1->type_.type_, t2 = (int) e2->type_.type_;

  if (t1 < t2) {
    reporter_.Error("Could not cast right expression", e1, e2, e1->op_);
    e1->type_.type_ = TokenType::T_NONE;
    return e1->type_;
  }

  return e1->type_;
}

Type& TypeChecker::PromotePrim(ExprRef e1, ExprRef e2) {
  int t1 = (int) e1->type_.type_, t2 = (int) e2->type_.type_;
  return t1 > t2 ? e1->type_ : e2->type_;
}

// #FIXME: code gen of conditions with '!'
void TypeChecker::Visit(const std::shared_ptr<Unary>& unary) {
  unary->expr_->is_const_ = unary->is_const_;

  switch (unary->op_->GetType()) {
    case TokenType::T_PLUS:
    case TokenType::T_MINUS:
    case TokenType::T_NOT:
    case TokenType::T_NEG: {
      unary->expr_->Accept(*this);
      unary->type_ = unary->expr_->type_;
      return;
    }
    case TokenType::T_INC:
    case TokenType::T_DEC:
    case TokenType::T_BIT_AND: {
      unary->expr_->return_ptr_ = true;
      unary->expr_->Accept(*this);

      if (!unary->expr_->IsLvalue()) {
        reporter_.Report("Lvalue required for prefix operand", unary->op_);
        return;
      }

      // inc/dec on array is invalid
      if ((unary->op_->GetType() == TokenType::T_INC || unary->op_->GetType() == TokenType::T_DEC)
          && unary->expr_->is_indexable_) {
        reporter_.Report("Increment or decrement operations are invalid for arrays", unary->op_);
        return;
      }

      unary->to_scale_ = unary->expr_->type_.IsPointer();
      unary->type_ = unary->expr_->type_;
      if (unary->op_->GetType() == TokenType::T_BIT_AND)
        unary->type_.ind++;
      return;
    }
    case TokenType::T_STAR: {
      unary->expr_->Accept(*this);

      if (!IsPointer(unary->expr_)) {
        reporter_.Report("Expression to '*' must be a pointer", unary->op_);
        return;
      }

      unary->is_lvalue_ = true;
      unary->type_ = unary->expr_->type_;
      unary->type_.len = 0;
      unary->type_.ind = std::max(unary->type_.ind - 1, 0);
      return;
    }
    default: {
      reporter_.Report("Invalid operator in 'unary' expression", unary->op_);
      return;
    }
  }
}

void TypeChecker::Visit(const std::shared_ptr<Access>& access) {
  access->name_->return_ptr_ = true;
  access->name_->Accept(*this);

  if (!access->name_->type_.IsStruct() && !access->name_->type_.IsUnion()) {
    reporter_.Report("Only struct/union fields can be accessed via '.' or '->' operator", access->op_);
  }

  if (access->op_->GetType() == TokenType::T_DOT && access->name_->type_.ind != 0) {
    reporter_.Report("Struct variable expected, but got pointer", access->op_);
  }

  if (access->op_->GetType() == TokenType::T_ARROW && access->name_->type_.ind != 1) {
    reporter_.Report("Pointer to struct expected", access->op_);
  }

  if (!access->field_->IsVariable()) {
    reporter_.Report("Only string fields are supported", access->field_->op_);
  }

  access->is_lvalue_ = true;
  const std::shared_ptr<Literal>& field = std::static_pointer_cast<Literal>(access->field_);
  Entry* field_ = symbol_table_.GetField(access->name_->type_, field->op_->String());

  if (field_ == nullptr) {
    reporter_.Report("Undefined field", field->op_);
    return;
  }

  access->type_ = *field_->type;
  field->type_ = *field_->type;
  field->offset_ = field_->offset; // annotate node with offset value
}

void TypeChecker::Visit(const std::shared_ptr<Binary>& binary) {
  binary->left_->is_const_ = binary->is_const_;
  binary->right_->is_const_ = binary->is_const_;
  binary->left_->Accept(*this);
  binary->right_->Accept(*this);
  MatchTypes(binary->left_, binary->right_, false, binary);
}

void TypeChecker::Visit(const std::shared_ptr<Assign>& assign) {
  assign->left_->is_const_ = assign->is_const_;
  assign->right_->is_const_ = assign->is_const_;
  assign->left_->return_ptr_ = true;
  assign->left_->Accept(*this);

  if (!assign->left_->IsLvalue()) {
    reporter_.Report("Assign to rvalue is not allowed", assign->op_);
    return;
  }

  assign->right_->Accept(*this);
  MatchTypes(assign->left_, assign->right_, true, assign);
}

void TypeChecker::TypeCheck(const std::vector<std::shared_ptr<Stmt>>& stmts) {
  for (const auto& stmt : stmts) {
    stmt->Accept(*this);
  }
}

void TypeChecker::Visit(const std::shared_ptr<Literal>& literal) {
  if (literal->op_->GetType() == TokenType::T_IDENTIFIER) {
    if (literal->is_const_) {
      reporter_.Report("Variables initializers '"
          + literal->op_->String() + "' must be constant", literal->op_);
      return;
    }

    Entry* var = symbol_table_.Get(literal->op_->String());

    if (var == nullptr) {
      reporter_.Report("Variable '"
          + literal->op_->String() + "' has not been declared", literal->op_);
      return;
    }

    if (literal->is_function_ && var->func == nullptr) {
      reporter_.Report("'" + literal->op_->String() + "' is not a function", literal->op_);
      return;
    }

    if (literal->is_indexable_ && !var->type->IsArray() && !var->type->IsPointer()) {
      reporter_.Report("Invalid type for index operation, pointer or array expected", literal->op_);
      return;
    }

    literal->type_ = *var->type;
    literal->is_local_ = var->is_local;
    literal->offset_ = var->offset;

    // usage of array as pointer, literal->is_indexable_ is false, set to true
    // so usage *(array + 5) = 10, will return pointer to array
    if (!literal->is_indexable_ && var->type->IsArray()) {
      literal->is_indexable_ = true;
      literal->type_.ind++;
    }
  } else if (literal->op_->GetType() == TokenType::T_STR_LIT) {
    literal->type_.type_ = TokenType::T_CHAR;
    literal->type_.ind = 1;
    code_gen_.strings_[literal->op_->String()] = -1;
  } else { // integer
    int val = literal->op_->Int();
    literal->type_.type_ = (val <= 255 && val >= -256) ? TokenType::T_CHAR : TokenType::T_INT;
  }
}

// #TODO: fix error reporting
void TypeChecker::Visit(const std::shared_ptr<Print>& print) {
  print->expr_->Accept(*this);

  if (!IsIntegerType(print->expr_)) {
    reporter_.Report("'print' statement expects integer", print->token_);
  }
}

void TypeChecker::Visit(const std::shared_ptr<ExpressionStmt>& expr_stmt) {
  expr_stmt->expr_->Accept(*this);
}

void TypeChecker::Visit(const std::shared_ptr<Conditional>& cond_stmt) {
  cond_stmt->condition_->Accept(*this);

  if (!IsIntegerType(cond_stmt->condition_) && !IsPointer(cond_stmt->condition_)) {
    reporter_.Report("Used not-scalar where scalar is required", cond_stmt->token_);
    return;
  }

  cond_stmt->then_block_->Accept(*this);

  if (cond_stmt->else_block_ != nullptr) {
    cond_stmt->else_block_->Accept(*this);
  }
}

void TypeChecker::Visit(const std::shared_ptr<Block>& block_stmt) {
  symbol_table_.NewScope();

  if (gen_params_) { // parameter generation for functions
    int param_offset = 8;

    for (int i = 0; i < curr_func_->signature_->var_decl_list_.size(); i++) {
      const auto& decl = curr_func_->signature_->var_decl_list_[i];
      int offset = (i < 6) ? GetLocalOffset(decl->var_type_, 1) : (param_offset += 8);
      decl->offset_ = offset;
      symbol_table_.PutLocal(decl->name_->String(), &decl->var_type_, offset);
    }

    gen_params_ = false;
  }

  for (const auto& stmt : block_stmt->stmts_)
    stmt->Accept(*this);
  symbol_table_.EndScope();
}

void TypeChecker::Visit(const std::shared_ptr<While>& while_stmt) {
  while_stmt->condition_->Accept(*this);

  if (!IsIntegerType(while_stmt->condition_) && !IsPointer(while_stmt->condition_)) {
    reporter_.Report("Used not-scalar where scalar is required", while_stmt->token_);
    return;
  }

  while_stmt->loop_block_->Accept(*this);
}

void TypeChecker::Visit(const std::shared_ptr<For>& for_stmt) {
  symbol_table_.NewScope();
  for_stmt->init_->Accept(*this);

  if (for_stmt->condition_ != nullptr) {
    for_stmt->condition_->Accept(*this);
  }

  if (for_stmt->condition_ != nullptr && !IsIntegerType(for_stmt->condition_) && !IsPointer(for_stmt->condition_)) {
    reporter_.Report("Used not-scalar where scalar is required", for_stmt->token_);
    return;
  }

  for_stmt->loop_block_->Accept(*this);
  for_stmt->update_->Accept(*this);
  symbol_table_.EndScope();
}

void TypeChecker::Visit(const std::shared_ptr<DeclList>& decl_list) {
  for (const auto& var_decl : decl_list->var_decl_list_)
    var_decl->Accept(*this);
}

void TypeChecker::Visit(const std::shared_ptr<ExprList>& expr_list) {
  for (const auto& expr : expr_list->expr_list_)
    expr->Accept(*this);
}

void TypeChecker::Visit(const std::shared_ptr<ControlFlow>& flow_stmt) { }

// #FIXME: add support for `return;`
void TypeChecker::Visit(const std::shared_ptr<Return>& return_stmt) {
  return_stmt->expr_->Accept(*this);

  if (curr_func_->return_type_.IsVoid()) {
    reporter_.Error("Declared return type is 'void', but return statement found",
                    curr_func_->return_type_,
                    return_stmt->expr_,
                    return_stmt->token_);
    return;
  }

  if (!MatchTypeInit(curr_func_->return_type_, return_stmt->expr_)) {
    reporter_.Error("Declared type of init expression does not correspond to inferred type ",
                    curr_func_->return_type_, return_stmt->expr_, return_stmt->token_);
  }
}

void TypeChecker::RevertOffsets(Entry *fields, int size) {
  while (fields != nullptr) {
    fields->offset += size;
    fields = fields->next;
  }
}

void TypeChecker::Visit(const std::shared_ptr<Union>& decl) {
  // 1. unnamed union `union { int x; };`
  // 2. named union `union Book { int length; char* title; };`
  // 3. named union with var decl `union Book { int length; char* title; } union;
  if (decl->type_.name == nullptr && decl->var_name_ == nullptr) {
    reporter_.Warning("Unnamed union declarations are not supported", decl->token_);
    return;
  }

  // create a linked list of field declarations and validate them
  std::unordered_map<std::string, int> offsets;
  Entry *fields = new Entry, *head = fields;
  int offset = 0;
  for (const auto& var_decl : decl->body_->var_decl_list_) {
    auto *field = new Entry;
    const std::string& name = var_decl->name_->String();
    int len = var_decl->var_type_.len; // #FIXME: arrays in unions (array indexing is broken)

    if (offsets.count(name) != 0) {
      reporter_.Report("Redefinition of field in struct", var_decl->name_);
      FreeEntries(head);
      return;
    }

    offset = std::min(GetOffset(var_decl->var_type_, len), offset); // find max offset (min, as offsets are negative)
    offsets[name] = offset;
    *field = {&var_decl->var_type_, false, 0, nullptr, nullptr, name};
    fields->next = field;
    fields = fields->next;
  }

  fields->next = nullptr;
  decl->size = (head == fields) ? 0 : std::abs(offset);
  fields = head;
  head = head->next;
  free(fields);

  std::string name;

  if (decl->type_.name == nullptr) {
    name = "__unnamed_union__" + std::to_string(unnamed++);
    decl->type_.name = std::make_shared<Token>();
    decl->type_.name->SetString(name);
  } else {
    name = decl->type_.name->String();
  }

  if (symbol_table_.ContainsType(name)) {
    reporter_.Report("Union has already been declared", decl->type_.name);
    return;
  }

  symbol_table_.PutType(name, decl->size, head);

  if (decl->var_name_ != nullptr) {
    symbol_table_.PutGlobal(decl->var_name_->String(), &decl->type_, nullptr);
  }
}

void TypeChecker::Visit(const std::shared_ptr<Struct>& decl) {
  // 1. unnamed struct `struct { int x; };`
  // 2. named struct `struct Book { int length; char* title; };`
  // 3. named struct with var decl `struct Book { int length; char* title; } book;
  if (decl->type_.name == nullptr && decl->var_name_ == nullptr) {
    reporter_.Warning("Unnamed struct declarations are not supported", decl->token_);
    return;
  }

  // create a linked list of field declarations and validate them
  std::unordered_map<std::string, int> offsets;
  Entry *fields = new Entry, *head = fields;
  int offset = 0;
  for (const auto& var_decl : decl->body_->var_decl_list_) {
    auto *field = new Entry;
    const std::string& name = var_decl->name_->String();
    int len = var_decl->var_type_.len; // #FIXME: arrays in structs

    if (offsets.count(name) != 0) {
      reporter_.Report("Redefinition of field in struct", var_decl->name_);
      FreeEntries(head);
      return;
    }

    offset += GetOffset(var_decl->var_type_, len);
    offsets[name] = offset;
    *field = {&var_decl->var_type_, false, offset, nullptr, nullptr, name};
    fields->next = field;
    fields = fields->next;
  }

  fields->next = nullptr;
  decl->size = (head == fields) ? 0 : std::abs(offset);
  fields = head;
  head = head->next;
  free(fields);

  RevertOffsets(head, decl->size); // offset shows the last address, revert fields offsets to be positive

  std::string name;

  if (decl->type_.name == nullptr) {
    name = "__unnamed_struct__" + std::to_string(unnamed++);
    decl->type_.name = std::make_shared<Token>();
    decl->type_.name->SetString(name);
  } else {
    name = decl->type_.name->String();
  }

  if (symbol_table_.ContainsType(name)) {
    reporter_.Report("Struct has already been declared", decl->type_.name);
    return;
  }

  symbol_table_.PutType(name, decl->size, head);

  if (decl->var_name_ != nullptr) {
    symbol_table_.PutGlobal(decl->var_name_->String(), &decl->type_, nullptr);
  }
}

void TypeChecker::Visit(const std::shared_ptr<FuncDecl>& func_decl) {
  ResetLocals();

  Entry* func = symbol_table_.Get(func_decl->name_->String());

  if (func == nullptr) {
    symbol_table_.Put(func_decl);
  } else {
    reporter_.Report("Function '" + func_decl->name_->String() + "' redefinition", func_decl->name_);
    return;
  }

  if (func_decl->body_ != nullptr) { // not prototype
    NewLabelScope(func_decl);
    curr_func_ = func_decl;
    gen_params_ = true;
    func_decl->body_->Accept(*this);
    func_decl->local_offset_ = local_offset_;
    curr_func_ = nullptr;
    CheckLabelScope(func_decl);
  }
}

// int array[25];
void TypeChecker::Visit(const std::shared_ptr<VarDecl>& decl) {
  Entry* var = symbol_table_.GetLocal(decl->name_->String());

  if (decl->var_type_.IsVoid() && !decl->var_type_.IsPointer()) {
    reporter_.Report("Variable '" + decl->name_->String() + "' has unallowable type 'void'", decl->name_);
    return;
  }

  if (var != nullptr) {
    reporter_.Report("Variable '" + decl->name_->String() + "' has already been declared", decl->name_);
    return;
  }

  if (decl->is_local_) {
    int len = decl->var_type_.len == 0 ? 1 : decl->var_type_.len;
    int offset = GetLocalOffset(decl->var_type_, len);
    decl->offset_ = offset;
    symbol_table_.PutLocal(decl->name_->String(), &decl->var_type_, offset);
  } else {
    symbol_table_.PutGlobal(decl->name_->String(), &decl->var_type_, nullptr);
    if (decl->var_type_.IsStruct() || decl->var_type_.IsUnion()) {
      const std::string& st_name = decl->var_type_.name->String();
      if (!symbol_table_.ContainsType(st_name)) {
        reporter_.Report("Composite (union/struct) '" + decl->var_type_.name->String() + "' has not been declared", decl->var_type_.name);
        return;
      }
    }
  }

  if (decl->init_ != nullptr) { // #TODO: reconsider const initializer and code gen for them
    decl->init_->is_const_ = decl->is_const_init_;
    decl->init_->Accept(*this);
    if (!MatchTypeInit(decl->var_type_, decl->init_)) {
      reporter_.Error("The type of init expression does not match the declared type of the variable ",
                      decl->var_type_, decl->init_, decl->name_);
    }
  }
}

void TypeChecker::Visit(const std::shared_ptr<Call>& call) {
  call->name_->is_function_ = true;
  call->name_->Accept(*this);
  call->type_ = call->name_->type_;

  const std::shared_ptr<Literal>& literal = std::static_pointer_cast<Literal>(call->name_);
  Entry* descr = symbol_table_.Get(literal->op_->String());
  if (descr == nullptr) return;

  FuncDecl* func = descr->func;

  if (call->args_->expr_list_.size() != func->signature_->var_decl_list_.size()) {
    reporter_.Report("Number of arguments does not correspond to declared number of parameters", literal->op_);
    return;
  }

  for (int i = 0; i < call->args_->expr_list_.size(); i++) {
    const auto& arg   = call->args_->expr_list_[i];
    const auto& param = func->signature_->var_decl_list_[i];
    arg->Accept(*this);
    if (!MatchTypeInit(param->var_type_, arg)) {
      reporter_.Error("Declared type of parameter does not correspond to inferred argument type ",
                      param->var_type_, arg, arg->op_);
    }
  }
}

void TypeChecker::Visit(const std::shared_ptr<Index>& index) {
  index->name_->is_indexable_ = true;
  index->name_->Accept(*this);
  index->index_->Accept(*this);
  index->index_->to_scale_ = true;
  index->is_lvalue_ = true;

  if (!IsIntegerType(index->index_)) { // #FIXME: could pointers be used as index?
    reporter_.Report("Index must be an integer type", index->name_->op_);
    return;
  }

  index->type_= index->name_->type_;
  index->type_.ind += index->name_->type_.IsArray() ? 0 : -1; // fix for pointer indexing
  index->type_.len = 0;
}

void TypeChecker::Visit(const std::shared_ptr<Grouping>& grouping) {
  grouping->expr_->is_const_ = grouping->is_const_;
  grouping->expr_->return_ptr_ = grouping->return_ptr_;

  grouping->expr_->Accept(*this);

  grouping->is_lvalue_ = grouping->expr_->is_lvalue_;
  grouping->type_ = grouping->expr_->type_;
}

void TypeChecker::Visit(const std::shared_ptr<Ternary>& ternary) {
  ternary->condition_->is_const_ = ternary->is_const_;
  ternary->then_->is_const_ = ternary->is_const_;
  ternary->else_->is_const_ = ternary->is_const_;
  ternary->condition_->Accept(*this);
  ternary->then_->Accept(*this);
  ternary->else_->Accept(*this);
  MatchTypes(ternary->then_, ternary->else_, false, ternary);
}

void TypeChecker::Visit(const std::shared_ptr<Label>& label) {
  label->label_ = code_gen_.GetLabel();
  const std::string& name = label->token_->String();
  if (code_gen_.labels_.count(name) > 0) {
    reporter_.Report("Label redefinition", label->token_);
    return;
  }
  code_gen_.labels_[curr_func_->name_->String()][label->token_->String()] = label->label_;
}

void TypeChecker::Visit(const std::shared_ptr<GoTo>& go_to) {
  const std::string& name = go_to->token_->String();
  if (code_gen_.labels_[curr_func_->name_->String()].count(name) == 0)
    code_gen_.labels_[curr_func_->name_->String()][go_to->token_->String()] = -1;
}

void TypeChecker::NewLabelScope(const std::shared_ptr<FuncDecl>& func_decl) {
  code_gen_.labels_[func_decl->name_->String()] = {};
}

void TypeChecker::CheckLabelScope(const std::shared_ptr<FuncDecl>& func_decl) {
  const std::string& name = func_decl->name_->String();
  for (const auto& pair : code_gen_.labels_[name]) {
    if (pair.second == -1) {
      reporter_.Report("Label '" + pair.first + "' used, but not declared, in function", func_decl->name_);
    }
  }
}

void TypeChecker::Visit(const std::shared_ptr<Postfix>& postfix) {
  postfix->expr_->is_const_ = postfix->is_const_; // #FIXME: throw error immediately, as postfix is lvalue?
  postfix->expr_->return_ptr_ = true;
  postfix->expr_->Accept(*this);

  if (!postfix->expr_->IsLvalue()) {
    reporter_.Report("Lvalue required for postfix operand", postfix->op_);
    return;
  }

  // inc/dec on array is invalid
  if ((postfix->op_->GetType() == TokenType::T_INC || postfix->op_->GetType() == TokenType::T_DEC)
      && postfix->expr_->is_indexable_) {
    reporter_.Report("Increment or decrement operations are invalid for arrays", postfix->op_);
    return;
  }

  postfix->to_scale_ = postfix->expr_->type_.IsPointer(); // scale for pointer
  postfix->is_lvalue_ = false;
  postfix->type_ = postfix->expr_->type_;
}

bool TypeChecker::IsComparison(TokenType type) {
  return type == TokenType::T_EQUALS || type == TokenType::T_NOT_EQUALS
    || type == TokenType::T_LESS || type == TokenType::T_LESS_EQUAL
    || type == TokenType::T_GREATER || type == TokenType::T_GREATER_EQUAL;
}

void TypeChecker::FreeEntries(Entry *entry) {
  while (entry != nullptr) {
    Entry *curr = entry;
    entry = entry->next;
    free(curr);
  }
}

int TypeChecker::GetOffset(const Type& type, int len) {
  len = len == 0 ? 1 : len;
  if (type.IsPointer()) return - len * 8;
  return - len * GetTypeSize(type);
}

int TypeChecker::GetLocalOffset(const Type& type, int len) {
  if (type.IsPointer()) return - (local_offset_ += 8 * len);
  local_offset_ += len * (GetTypeSize(type) > 4 ? GetTypeSize(type) : 4);
  return - local_offset_;
}

int TypeChecker::GetTypeSize(const Type& type) {
  if (type.IsStruct() || type.IsUnion()) return symbol_table_.GetType(type.name->String())->size;
  switch (type.type_) {
    case TokenType::T_CHAR: return 1;
    case TokenType::T_SHORT: return 2;
    case TokenType::T_INT: return 4;
    case TokenType::T_LONG: return 8;
    default: {
      std::cerr << "Internal error: invalid type." << std::endl;
      exit(1);
    }
  }
}
void TypeChecker::ResetLocals() {
  local_offset_ = 0;
}

} // namespace mcc
