compiler: main.cc src/Scanner.cc src/TokenType.cc src/ast.cc src/Parser.cc src/CodeGenX86.cc
	g++ -o compiler -g src/TokenType.cc src/Scanner.cc src/ast.cc src/Parser.cc main.cc src/CodeGenX86.cc

clean:
	rm -f compiler *.o
