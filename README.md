# PL/0 Compiler

A PL/0 compiler implemented in C, featuring lexical analysis, parsing, code generation, and a virtual machine executor.

## Features

- **Lexical Analysis**: Tokenizes PL/0 source code
- **Parsing**: Syntax analysis with support for procedures, variables, constants, arrays
- **Code Generation**: Generates intermediate code for a stack-based virtual machine
- **Virtual Machine**: Executes the compiled PL/0 bytecode
- **Supported Constructs**:
  - Constants and variables
  - Procedures (with nested scopes)
  - Arrays (1-dimensional with configurable bounds)
  - Arithmetic expressions
  - Conditionals (`if-then`, `if-then-else`)
  - Loops (`while`, `for`-style via while)
  - Input/Output (`read`, `write`)
  - Comments (`{...}`)

## Project Structure

```
pl0-compiler/
├── src/              # Source code
│   ├── main.c        # Entry point
│   ├── lexer.c       # Lexical analysis
│   ├── parser.c      # Syntax analysis
│   ├── codegen.c     # Code generation
│   ├── vm.c          # Virtual machine
│   ├── symbol.c      # Symbol table management
│   ├── platform.c    # Error handling & listing
│   ├── set.c         # Set operations
│   └── pl0.h         # Header file
├── Examples/          # Sample PL/0 programs
├── Makefile
├── README.md
└── .gitignore
```

## Build

```bash
make
```

## Run

```bash
./pl0
```

The compiler will prompt for:
1. Input file name
2. Whether to list object code (Y/N)
3. Whether to list symbol table (Y/N)

## Example

```pl/0
var f, c;

begin
    read(f);
    c := 5 * (f - 32) / 9;
    write(c)
end.
```

## License

MIT