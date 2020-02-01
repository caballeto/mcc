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

## Day 3

I implemented three new features today:

- represent `Assign` as an expression, such things as `x = y = 1 + 120 * 4` are
supported
- comparison operators (`==`, `!=`, `<`, `<=`, `>`, `>=`)
- block statement - starts with '{' then set of statements, then '}'
- if statement - support for if and else clause
```c
if (expr) {
  stmts
} else {
  stmts
}
```
- added evaluation of condition `expr`. If it is zero, then
jump to false label, else don't jump and execute true branch statements. 

Besides, I have added several support functions dumping AST.
The compiler at the moment is a bit more looks like real C code,
as there is support for simple statements and blocks, but still
a lot more is ahead.

## **Next**
Further I think I would work on adding loops and types,
namely I would want to at least provide support for `short`, `int`, `char`
and `long`. Float and doubles will be left for the future.
Afterwards, other types like pointers and arrays (global).
And then, finally functions, function calls, scopes and local variables.

## Day 4

Today, after a several weeks break, I returned to the project. I did some few
minor changes, which I have been leaving for the future previously. Small amount
of code here, as I mainly try to recall the stuff I worked on, that might take some time.

Changes:

- implemented `AstDumper` class, which implements `Visitor` interface and dumps
    Ast tree into a file (maybe will change to dump into `std::ostream` instead)
- deleted previous `Dump` methods implementations in `ast.cc`
- fixed messed code `std::static_pointer_cast<T>(shared_from_this())` by creating
  template method `std::shared_from_base<T>`, which will create shared pointers of needed type
- fixed small bugs in ast dump methods

Tomorrow will start where I have previously left, namely loops, types, var inits, 
var declaration list. It will definitely take some time to get to functions.