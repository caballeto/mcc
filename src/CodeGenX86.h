//
// Created by vladyslav on 15.01.20.
//

#ifndef MCC_SRC_CODEGENX86_H_
#define MCC_SRC_CODEGENX86_H_

#include "common.h"
#include "ast.h"

#define REGISTER_NUM 8

namespace mcc {

class CodeGenX86 : public Visitor {
 public:
  explicit CodeGenX86(const std::string& output_file) {
    out_.open(output_file, std::ios::out);
  }

  void Generate(std::shared_ptr<Expr> expr);

 private:
  void Preamble();
  void Postamble();
  int Visit(std::shared_ptr<Binary> binary) override;
  int Visit(std::shared_ptr<Literal> literal) override;
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
