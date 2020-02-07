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

## Day 5

I have implemented following features today:

- `while (...) { ... }`, `do { ... } while (...);` loops
- variable declaration list with optional initializers (non-constant)
  `int x = 1 + 2, y = 25, d = x - y;`
- `for` loops, with support for init lists

```c
for (int i = 0, j = 0; i < 10; i = i + 1, j = j + 1) {
  print i;
  print k;
  print i + j;
}
```

All variables are global for now, that will be changed once functions will
be implemented.
- expression lists, will be useful for parsing argument lists
  `expr1, expr2, expr3, ..., exprn`
  
I think I will also add today support for loops with one statements without
brackets `while (expr) stmt;`, `do stmt while (expr);`, `for (;;) stmt`. Also,
I will make expressions in for loops optional, so things like `for (;;)` will be 
valid.

## **Next**
Same as previous.

## Day 6

Implemented:

- break/continue control flow statements
- added `ControlFlowChecker` to find control flow statements
- allowed `if` with single statements
- allowed no condition in `for` loops

Next tasks for today: 

- [X] add `TypeChecker` visitor to make type checks. [no checks]
- [X] add `ErrorReporter` to report errors correctly
- add `int`, `short`, `long`, `char` types and add cg for them
- (?) add `Compiler` class for export into `main.cc`

Example:

```c
for (int i = 0, j = 0; i < 10; i = i + 1, j = j + 1) {
  if (i == 2) {
    continue;
  } else {
    print i * j;
  }
}

int x = 0;
while (x < 10) {
  if (x == 6) break;
  x = x + 1;
  print x;
}
```

## Day 7

Today, I worked mainly on adding TypeChecker and adding simple types
to the language. For now only `short`, `int` and `long` are supported.
I have made visitor a template class, so there will be less code duplication.
`TypeChecker` implements `Visitor<Type>`, where `Type` is an enum which will
represent valid types (`VOID`, `INT`, `SHORT`, `LONG`; `STRUCT` in future).

Summary below:

- added type keywords to scanner
- create `enum class Type` for type representation
- edited `SymbolTable` to save variable type
- edited `Parser` to parse different types and save them to symbol table
- create `is_lvalue` field to check whether you can assign to expression
- create `TypeChecker` class to make semantic checks
- added routines for simple type cast hierarchy (short -> int -> long)
- added new methods to ErrorReporter for error printing (still more work ahead)

## **Next**

For the next several days.

- rewrite `ErrorReporter` to support standardized error formats (lex/parse/semantic)
- decide how to correctly output errors, when to stop
- add sync mechanism to `Parser`
- make `CodeGen` class to use type annotations and generate code based on them
- add pointers
- refactor code and add testing

## Day 8

So far, I have implemented the following:

- rewrote `ErrorReporter` to print fine error messages, which are much better now
- extended ast nodes to take token to preserve info about line and char count in case of error
- edited all parts of compiler to use `ErrorReporter` 
- added sync mechanism to `Parser`
- added more tests for error cases

More tasks for today:

- [x] implement pointers (parse + typecheck + dereference/assign ops)
- [x] think about where to catch `ParseException`, so as to avoid cascading errors
- [ ] add labels/goto if will have time
- [x] automatic tests with python script

Edit: have started implementing pointers, but they are more complex than expected.
Dereferencing and taking address is implemented. Assign to address is left for tomorrow.

## Day 9

Today, I worked on adding testing features, which took lots of time. Mainly
trying to figure out how to work with stream redirections in python/c++/shell
in order to compare output correctly. The final program is still pretty bad,
but I will edit it somewhat later.

Implemented:

- pointers dereferencing, taking address, assigning to pointer
- assigning lvalue flags
- parsing parameter list
- function definition (zero parameters only)
- storing function with no parameters in symbol table (other parameters ignored)
- code generation for functions definitions
- return statements (no check for function with no return)
- checks for compatible return type, return of 'void'

## **Next**

Well, there are lots of features to work on, I getting an understanding
that it is indeed a challengable project. Features for the near future:

- [x] functions calls with no arguments
- [x] loading return value of function calls
- more operators (logical, bitwise, grouping, ++/-- for pointers also)
- strings and arrays
- local variables
- passing function call parameters
- general function calls (syntax, typecheck, code gen, etc.)
- labels/goto

Also, thoughts on optimization:

- make a bitfield, which will store flags used during tree traversal
- add optimizer which will prune the tree
  1. Eliminate unreachable code.
  2. Eliminate unused parts of code (that do not have effects).
  3. Fold constants and useless operators (e. g. &*p1 == p1).
  4. Add checks for last statements, in order to generate less labels/code.
  5. Add more checks for eliminating useless instructions.

```c
int func() {
  int y = 5;
  if (y == 5) {
    return 25;
  } else {
    return 1;
  }
}

int main() {
  int some_int = 25;
  print some_int * 10;
  return some_int;
}
```

## Day 10

Had little time to work today due to interview. Implemented following:

- support for different type sizes of variables (all intermediate values are `long` 64 bit)
- added `hack` support of pre-increment operator of variables

Tasks for tomorrow:

- give good design consideration to parsing prefix/postfix operators
- think about lvalue/rvalue flags and implementation of increment with pointers/vars
- implement many other operators (mentioned above)
- goto/labels

Overall after tomorrow tasks, I should be ready to write local variables - task for Sunday.
Rewrite SymbolTable and allow functions calls/function parameters on Monday



