//
// Created by vladyslav on 15.01.20.
//

#ifndef MCC_SRC_CODEGENX86_H_
#define MCC_SRC_CODEGENX86_H_

#include "common.h"
#include "ast.h"
#include "Visitor.h"

#define REGISTER_NUM 8
#define NO_RETURN -1

namespace mcc {

class CodeGenX86 : public Visitor {
 public:
  explicit CodeGenX86(const std::string& output_file) {
    out_.open(output_file, std::ios::out);
  }

  void Generate(const std::vector<std::shared_ptr<Stmt>>& stmts);

 private:
  int Visit(const std::shared_ptr<Binary>& binary) override;
  int Visit(const std::shared_ptr<Literal>& literal) override;
  int Visit(const std::shared_ptr<Print>& print) override;

  void Preamble();
  void Postamble();

  int NewRegister();
  void FreeRegister(int reg);
  void PrintInt(int reg);

  std::ofstream out_;

  std::vector<std::string> rregisters = {"%r8", "%r9", "%r10", "%r11",
                                          "%r12", "%r13", "%r14", "%r15"};
  std::vector<std::string> dregisters = {"%r8d", "%r9d", "%r10d", "%r11d",
                                          "%r12d", "%r13d", "%r14d", "%r15d"};
  std::vector<std::string> bregisters = {"%r8b", "%r9b", "%r10b", "%r11b",
                                          "%r12b", "%r13b", "%r14b", "%r15b"};

  bool regs_status[8] = {true, true, true, true, true, true, true, true};
};

} // namespace mcc

#endif //MCC_SRC_CODEGENX86_H_
