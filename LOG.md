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

## Day 11

First part is done. Implemented parsing of many new prefix/postfix operators.
Only && and || were left out due to complexity. Now increment and decrement works
with both variables and dereferenced pointers (`(*x)++`), but scaling when
adding to pointers has not been added yet.

More tasks for today:

- [x] add pointer scaling
- [x] rewrite all tests and add new ones (redirect everything to standard output, regenerate tests)

Fixed pointers increment/decrement issues, rewrote current tests and updated
`run_tests.py` to compile files and compare output. Code is not in best shape,
but at least it works as intended. There was confusion about functions `IsVariable()` and
`IsPointer()`, so I will consider rewriting these and making a clear distinction
on what they mean.

Also, implemented and tested ternary operators, negative numbers. Haven't tested other
binary/unary operators like `if (!ptr) { ... }`, but they should work theoretically.

## **Next**

- char type and character representation
- strings as char* and string pointer iteration (`for (p = "string"; *p; p++)`)
- arrays (one dimensional for now), type checking
- subscription in arrays and integrating them with available operators (`x[1]++`)

Extra tasks:

- add logical operators
- add tests for operators (logical, bitwise, not, negation)
- adding some bigger program tests

## Day 12

Fixed scaling issues in binary operators. Improved error reporting by
adding `warning` messages for comparison/assignment between int,long and pointer types.
Also improved handling of scanner errors, also it still throws errors.

- fixed scaling issues
- partially implemented one-dimensional arrays, more tests needed
- work with primitive types, but not generalize to pointer arrays yet.
- rewrote parser to represent calls and index operations as expressions
- simple tests do path

## Day 13

Implemented following

- ended 1d array implementation for prim types
- added functionality for array to work with pointer
- following is supported: `int* ptr = array;`, `int* ptr = &array[0]`,
  `*(array + 1) = 1;`, `ptr[2] = 1`, `array[1] = 1`
- added warning for pointer type conversions
- made changing of array variable in inc/dec expressions invalid

- Implemented hacked version of program (declared print funcs manually, rewrote assembly)

TODO:

- [x] fix types in dereferencing namely, when moving char use
  `movzbq (%r8) %r8`, not `movq (%r8), %r8`, as it will print
  far out than zero char.
- [x] declare the globals first, go over program and declare
  global variables and anonymous strings in data section
- [x] then generate code for functions
- [ ] improve generating of conditional expressions (eliminate too many comparisons
  with top level check for conditional expression)
 
Finally implemented working program with strings. Fixed bugs with loading from pointer
and incorrect scanning. Now storage for strings is generated during type checking.
So code generator is passed to type checker. Not the most elegant, but the fastest solution.
Rewrote tests to use `printint` function, instead of `print` statement.

In conclusion, all major parts are working, there are definitely bugs in implementation, but
still it is working. That means that I can start working on local variables and function calls.
It will be the most essential part of compiler, so will have to take some time to design things
well. Cool, let's go!

## Day 14

### Local variables.

**Design notes**

Variable generation:
if variable is local, then `movq -8(%rbp), %r8`, if variable
is global `movq name(%rip), %r8`

```asm
pushq   %rbp
movq    %rsp, %rbp
subq    64, %rsp
...
leave
ret
```

Symbol table is a simple abstraction:

SymbolTable = std::vector<std::unordered_maps<std::string, Entry>>;

The typechecker will go over the tree and will save information
about the variable position on stack. Symbol table would be represented
as stack of maps, from name to `struct Entry` descriptor. The descriptor
of variable will store, whether the variable is local or is global.

The generating code will have a function for generating loading part of the 
instruction for given variable. It will get a variable descriptor, and
will generate appropriate code for loading.

The stack offset variable will store the general stack offset. The local
offset variable will store the local offset of function. These are variables
of TypeChecker, that will then store them in FuncDecl node and in individual
variables.

**Results**

Rewrote symbol table to support adding and removing scopes. Implemented local 
variables that support multiple scopes. They are stored on the stack with a minimum of 4-byte alignment.
Fixed bugs with loading from address and storing into address. 

**Next**

Params/arguments/local arrays are next on the list.

## Day 15

### Parameters

In `FuncDecl` class, generate code to move parameter variables from registers
onto the stack. See stack layout.

### Arguments

Arguments are loaded partially into registers (6), and the rest is loaded onto stack.
Think about stack spilling.

### **Results**

Implemented features:

- allocating space for 6 first parameters, which are passed in registers
- loading all other registers on stack, and saving offset
- function prototypes (signatures)
- type checking function calls (function exists, number and parameter types match)
- code gen for function calls
- added few tests (no tests with pointers)

**Next**

At this point it seems, that the major part of the compiler is done.
We can create functions and call them. I have tried to test an implementation
with recursive function, but figured out that it wouldn't work without register
spilling. In other cases, registers will be reassigned and correct values would be
lost. So the next tasks I would aim to do are following:

- local arrays
- register spilling
- switch
- preprocessor
- structs
- unions
- enums
- type casts
- sizeof
- extern, static
- refactoring, testing and small optimization

## Day 16

So, today I am going to do the following tasks:

- [X] local arrays
- [X] register spilling (very inefficient)
- [X] goto/labels

I have implemented all of the above features. The register spilling
is very inefficient, I will rewrite it to spill only used registers, instead
of all registers.

I will also have to spend some time of refactoring, because code generation
have many repeatable code fragments, which should be abbreviated by functions, like
generate label or generate jump, etc.

Not many features left (from the subset I aim to implement), so the goal
is to finish the compiler by Day 20.

**Next**

- refactoring code
- switch
- type casting

## Day 17

Started implementing structs. Wholly rewrote the representation of types.
Currently, only the declarations of global structs are supported. Tomorrow,
I will implement the generation of global variables and global unnamed strings first.
Then I will add local structs, and finally would add two operations for member
access '.' and '->'. I will add both assignment and just getting the variable.

## Day 18

Implemented the general structure, but failed to correctly write the structure offsets.
So, the values are being rewritten, hence I need to debug it and fix the offset problems.

## Day 19

Huh, I feel like I am making some progress. Today I have fixed the implementation of structs
and added unions. I have also fixed several bugs with arrays in structs/unions. In my implementation
unions and structs are almost identical, the difference only is in how much memory is allocated for them.
So, yeah, this part was tough, and I am sure that there are many hidden bugs in implementation, as
I have tested on straightforward examples. 

Following, I am going to work on adding `enum`, `switch` and conditional operators.

*Bug fix:*

- [**test 30**] fix index operation with pointer types `int* ptr = &array; ptr[2] = 10;`

## Day 20

- fixed bug with index operation on pointers
- fixed bug with parsing arrays in parameter list (not allowed)
- moved main running code into `Compiler` class, will rewrite it when
  CLI functionality will be added

- [X] enums

## Day 21

Implemented switches and runtime flags.

- [X] switch
- [X] runtime flags

**Next**

- [ ] preprocessor
- [ ] extern
- [ ] typedef
- [ ] type casts
- [ ] sizeof
- [ ] static
- [ ] conditions & const init & refactoring

## Day 22

**Typedef notes**

typedef char* Str; 

Str s1, s2, s3; // s1 -> char*, s2 -> char*, s3 -> char*

struct Book { };

typedef int INT;
typedef struct Book B;

typedef struct { 
    int x;
    int y; 
    int z;
} Point;  

name -> &Type

### **Results**

- [x] typedef with primitives, union, struct, recursive typedef
  (no enums, arrays)
- [x] type casts
- [x] sizeof
- [x] extern
- [x] static
- [ ] preprocessor

**Bug**

- [x] fix sizeof incorrect behaviour with array

**Next**

- preprocessor
- global variable init & fold optimization
- conditional expression, &&, ||
- more assign operators (+=, -=, %+, etc.)
- refactoring
- test on some bigger programs

## Day 23

- [x] preprocessor & header files
- [x] fold optimization
- [x] global var init
- [ ] conditionals
- [ ] assign operators
- [x] refactoring & test

**Bug**

- call inside arguments reassigns `%rdi` (param) registers