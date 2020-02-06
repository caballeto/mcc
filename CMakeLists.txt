cmake_minimum_required(VERSION 3.15)
project(mcc)

set(CMAKE_CXX_STANDARD 17)

add_executable(compiler main.cc src/ast.cc src/CodeGenX86.cc src/Parser.cc src/Scanner.cc src/TokenType.cc src/SymbolTable.cc src/ControlFlowChecker.cc src/AstDumper.cc src/Type.cc src/ErrorReporter.cc src/TypeChecker.cc)