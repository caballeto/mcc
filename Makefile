main: src/TokenType.cc src/Scanner.cc src/ast.cc src/Parser.cc main.cc src/CodeGenX86.cc src/SymbolTable.cc src/AstDumper.cc src/ControlFlowChecker.cc src/TypeChecker.cc src/ErrorReporter.cc src/Type.cc src/Compiler.cc
	g++ -o compiler -g src/TokenType.cc src/Scanner.cc src/ast.cc src/Parser.cc main.cc src/CodeGenX86.cc src/SymbolTable.cc src/AstDumper.cc src/ControlFlowChecker.cc src/TypeChecker.cc src/ErrorReporter.cc src/Type.cc src/Compiler.cc

clean:
	rm -f mcc *.o
