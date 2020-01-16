# mcc

## Day 1

So far I have written basic structure of the compiler.
I am going to move ahead from here. I have done the following:

- TokenType - definitions of token types
- Token - definitions of token
- Scanner - scanner to scan input and output tokens one at a time
- AST - definitions of AST (Expr, Binary, Literal) + Visitor definition
- AST dump generation - output AST as xml into a file
- Code Generator - implementation of Visitor pattern using
    `shared_from_this()`, `std::static_pointer_cast()` and `std::shared_pointer`. 
    Generates basic operations (+, -, *, /) using X86 assembly.
    
The work so far was not hard, mainly trivial tasks of writing, outlining
the general structure of the program. The program compiler and works
correctly on simple programs. Haven't tested much thought. Have to add
more testing + automatic build routine.

### **Next**
Further I will work on implementing simple things like
more operators (parsing mainly), and then adding simple
constructs to a language, like `if/else` statements, loops,
blocks of code and global variable declaration.

## Day 2

Today I worked on implementing simple print statements and on declaration of global
variables (for now only of integer type). Following was implemented:

- print statements `print <expr>;`
- variable declaration `int <name>;`, only int type, no initialization/multiple variables
- assignment expression `x = <expr>`. `std::shared_ptr<Literal>` on left hand side,
    but as I remember, anything could be on right hand side (no type checking)
- usage of variables in expressions `print x + y;`, where `x` and `y` should be defined.
- added simple checks in variable usage expression (`Assign`, `Expression`) for previous
    variable definition
- created new `Stmt` definitions in `ast.h`, fixed things in `Visitor.h`
- started initial design of `SymbolTable` class, for now extra simple and inefficient (linear scan),
    will rebuilt in future (maybe multiple linked lists)

Had a few bugs, fixed them, compiled and run on basic example. Code so far
is working.

## **Next**
Further I will work on adding operators (comparison, bitwise, logic) and support
for basic C language constructs, namely:

- if/else statemets
- code blocks
- loops (for, while, do-while) 

Also, added several todos and things to think about in future.