main: src/TokenType.cc src/Scanner.cc src/ast.cc src/Parser.cc main.cc src/CodeGenX86.cc src/SymbolTable.cc src/AstDumper.cc
	g++ -o mcc -g src/TokenType.cc src/Scanner.cc src/ast.cc src/Parser.cc main.cc src/CodeGenX86.cc src/SymbolTable.cc src/AstDumper.cc

clean:
	rm -f mcc *.o