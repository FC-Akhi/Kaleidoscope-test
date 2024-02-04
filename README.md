# Kaleidoscope-test

## Introduction

Welcome to the Kaleidoscope-test repository! This project is a personal endeavor to implement the lexer, Abstract Syntax Tree (AST), and parser components of a compiler, guided by the Kaleidoscope tutorial. It's intended for educational purposes, allowing me to practice and understand the fundamentals of compiler design. 

## Installation Instructions

Before you can run the Kaleidoscope-test compiler, you need to ensure that you have the g++ compiler installed on your system. This project is developed and tested with g++, which is essential for compiling the C++ code.

**Prerequisites:**

- g++ compiler


## Usage

To compile and run the Kaleidoscope-test project, follow these steps:

1. Open a terminal or command prompt.
2. Navigate to the directory where `toy-compiler.cpp` is located.
3. Compile the code using the following command:

```sh
g++ toy-compiler.cpp -o test

```

Run with command:

```sh

./test

```

## Branches

The repository is organized into different branches, each serving a specific part of the project's development. The default branch is the master branch. Below are the name of branches available, along with their descriptions:

- Version_1: Parser handles number

	E.g: 89, 785, 5 etc.

- Version_2: Parser handles number and variable 

	E.g: 89, 785, 5, name, foo, employee etc.

- Version_3: Parser handles number, variable and function callee

	E.g: 89, 785, 5, name, foo, employee, foo(2, 3), printForMe(), foobar(a, b)  etc.

- Version_4: Parser handles number, variable, function callee and expressions with operators

	E.g: 89, 785, 5, name, foo, employee, foo(2, 3), printForMe(), foobar(a, b), ```(a+b*c)```  etc.

- Version_5: Parser handles number, variable, function callee, expressions with operators and function definition

	E.g: 89, 785, 5, name, foo, employee, foo(2, 3), printForMe(), foobar(a, b), ```(a+b*c)```, def foo(x)  etc.

- Version_6: Parser handles number, variable, function callee, expressions with operators, function definition and extern

	E.g: 89, 785, 5, name, foo, employee, foo(2, 3), printForMe(), foobar(a, b), ```(a+b*c)```, def foo(x), extern sin(arg) etc.


**Please navigate to the respective branches to explore more**