## What is it?
This is an interpreter with Pascal-like syntax, implemented with Flex and Bison libraries. 

To see example of program, use directory `examples`.

## Build and run
To build project run
```
make
```

To run interpreter use command
```
bin/pascal_interpret path/to/file/with/code
```

## Docs

Lexical analysis made with Flex(see file Lexer.l)
Syntax analysis made with Bison(grammar implement in file Parser.y)

### Build

0) Create directories `bin` and `build`
1) Code generation with Flex. Is create files `Lexer.cpp` and `Lexer.hpp` from file `Lexer.l`
2) Code generation with Bison. It create files `location.hh`, `position.hh`, `stack.hh`, `Parser.cpp` and `Parser.hpp` from file `Parser.y`
3) All created files and all other source files move to directory `build`
4) Compilation into `BIN_FILE`

## TODO

* Update documentation
* Fix shift/reduce conflicts
* Add LLVM ir generation from AST
