INCDIR=/tmp/include
BINDIR=/tmp

SRC= src/TokenType.cc src/Scanner.cc src/ast.cc src/Parser.cc main.cc src/CodeGenX86.cc src/SymbolTable.cc src/AstDumper.cc src/ControlFlowChecker.cc src/TypeChecker.cc src/ErrorReporter.cc src/Type.cc src/Compiler.cc src/Evaluator.cc

main: $(SRC)
	g++ -o compiler -g -DINCDIR=\"$(INCDIR)\" $(SRC)

install: compiler
	mkdir -p $(INCDIR)
	rsync -a include/. $(INCDIR)
	cp compiler $(BINDIR)
	chmod +x $(BINDIR)/compiler

clean:
	rm -f mcc *.o
