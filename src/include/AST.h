//===- AST.h - Node definition for the Kelidoscop AST ----------------------------===//
//
// This file implements the AST for the Kelidoscop language. It is optimized for
// simplicity, not efficiency. The AST forms a tree structure where each node
// references its children using std::unique_ptr<>.
//
//===----------------------------------------------------------------------===//

#ifndef LEXER_H
#define LEXER_H



#include "Lexer.h"
#include <vector>
#include <memory>



/// Here the type of inheritance used: Heirarchical

/// This is base class
class ExprAst {

public:

    ExprAst() {

        printf("ExprAst class\n");
    }

    /// Declare Destructor
    /// Why virtual???
    /// To achieve dynamic polymorphism. Which is the ability to call Derived class function using Base class pointer or reference
    /// By using virtual destructor in Base class we are making sure that we first call Derived class destructor and 
    /// then Base class destructor for the case - calling Derived class function using Base class pointer or reference
    virtual ~ExprAst() {} 
    virtual double getValue() const { throw std::runtime_error("getValue() not supported by this node type"); }

};



/// Derived class
class NumberExprAst : public ExprAst {

    double Val;

public:

    /// Declare Constructor
    /// double v is the constructore argument taking input
    /// : val(v) means member initializer list. Used to initialize class member which is val in here.
    NumberExprAst(double V) : 
                  Val(V) {

        printf("NumberExprAst class\n");
    
    } 

    double getValue() const { return Val; }
};



/// Derived class
class VariableExprAst : public ExprAst {

    std::string Name; 

public:

    /// Declare Constructor
    /// const std::string &n means reference parameter which is const and unchangable
    /// we will be not getting copy of the string but the string itself by referencing or using alias n
    /// : Name(N) this is member initializer list. Used to initialize class member which is Name in here.
    VariableExprAst(const std::string &N) : 
                    Name(N) {

        printf("VariableExprAst class\n");

    }
};



/// Derived class
class BinaryExprAst : public ExprAst {

    char Optr;  /// + - * / < >

    /// Creating 2 smart pointers capable of owning and managing objects of type ExprAst
    std::unique_ptr<ExprAst> LHS, RHS;

public:

    /// Declare constructor
    /// To initialize a BinaryExprAst object with a specific operator 
    /// and left-hand side (LHS) and right-hand side (RHS) expressions.
    BinaryExprAst(char Op, std::unique_ptr<ExprAst> LHS, std::unique_ptr<ExprAst> RHS) : 
                  Optr(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {      /// move: transfering the ownership of LHS came from 
                                                                            /// outside of the class to LHS inside of the class
        printf("BinaryExprAst class\n");

    }

};



/// Derived class
class CallExprAst : public ExprAst {

    std::string Callee;

    /// Its a vector (dynamic array) that holds multiple smart pointers. 
    /// Each of these smart pointers is of type std::unique_ptr<ExprAst>, meaning each one uniquely owns an ExprAst object.
    std::vector<std::unique_ptr<ExprAst>> Args;

public:

    CallExprAst(const std::string &C, std::vector<std::unique_ptr<ExprAst>> Args) : 
                Callee(C), Args(std::move(Args)) {

        printf("CallExprAst class\n");

    }

};



/// Another Base class
/// PrototypeAst - This class represents the "prototype" for a function,
/// which captures its name, and its argument names (thus implicitly the number
/// of arguments the function takes).
class PrototypeAst {
  
    std::string Name;

    
    std::vector<std::string> Args;

public:
    PrototypeAst(const std::string &N, std::vector<std::string> Args):  
                 Name(N), Args(std::move(Args)) {

        printf("PrototypeAst class\n");

    }


    const std::string &getName() const { return Name; }

};



/// Another Base class
/// FunctionAst - This class represents a function definition itself.
class FunctionAst {

    std::unique_ptr<PrototypeAst> Proto;
    std::unique_ptr<ExprAst> Body;

public:
    FunctionAst(std::unique_ptr<PrototypeAst> Proto, std::unique_ptr<ExprAst> Body) : 
                Proto(std::move(Proto)), Body(std::move(Body)) {

        printf("FunctionAst class\n");

  }

};



#endif /// LEXER_H






























