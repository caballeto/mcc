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