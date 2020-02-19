//
// Created by vladyslav on 18.02.20.
//

#ifndef MCC_SRC_COMPILER_H_
#define MCC_SRC_COMPILER_H_

#include "ast.h"
#include "AstDumper.h"
#include "common.h"
#include "ErrorReporter.h"
#include "ParseException.h"
#include "Parser.h"
#include "Scanner.h"
#include "SymbolTable.h"
#include "Token.h"
#include "TokenType.h"
#include "Type.h"
#include "TypeChecker.h"
#include "Visitor.h"

namespace mcc {

// #FIXME: rewrite when will add multiple file compilation
class Compiler {
 public:
  static int Compile(const std::string& input_file, const std::string& assembly_file);

  static bool fDumpAst;
  static bool fKeepAsm;
  static bool fAssemble;
  static bool fVerbose;
  static bool fLink;
};

} // namespace mcc

#endif //MCC_SRC_COMPILER_H_
