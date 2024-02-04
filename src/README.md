## Version_1 Description

This project is a basic implementation of a Kaleidoscope parser written in C++. It includes a lexer (lexer.h), a parser (parser.h), and abstract syntax tree definitions (ast.h) located in the include directory. The main entry point of the project is contained within toy-compiler.cpp, which starts the parsing process.


## Directory Structure

```
./
│
├── include/
│   ├── lexer.h
│   ├── parser.h
│   └── ast.h
│
└── toy-compiler.cpp
```

## EBNF Notation

The current version of parser is designed around the following EBNF notation, outlining the grammar for the simple language:

```
toplevelexpr   ::= expression
expression     ::= primary binoprhs
binoprhs       ::= ('+' | '-' | '*' | '/' | '<' | '>' primary)*
primary        ::= numberexpr | identifierexpr | parenexpr
numberexpr     ::= number
identifierexpr ::= identifier | identifier '(' expression* ')'
parenexpr      ::= '(' expression ')'

definition 	   ::= 'def' prototype expression
prototype      ::= identifer '(' identifer* ')'
```
**This EBNF notation is not complete. But, current version of the code is written on this notation only**


## Building the Project

To compile, ensure you have a C++ compiler installed (such as g++). Navigate to the src directory and run:

```
g++ toy-compiler.cpp -o toy-compiler
```

Run the executable:

```
./toy-compiler
```

