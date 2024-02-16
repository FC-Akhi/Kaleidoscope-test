//===- AST.h - Node definition for the Kelidoscop AST ----------------------------===//
//
// This file implements the AST for the Kelidoscop language. It is optimized for
// simplicity, not efficiency. The AST forms a tree structure where each node
// references its children using std::unique_ptr<>.
//
//===----------------------------------------------------------------------===//

#ifndef LEXER_H
#define LEXER_H

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include <map>


#include "Lexer.h"
#include <vector>
#include <memory>

using namespace llvm;



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
    virtual Value *codegen() = 0;




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

    Value *codegen() override;

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

    Value *codegen() override;
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

    Value *codegen() override;

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

    Value *codegen() override;

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

    Function *codegen();

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

  Function *codegen();

};








//===----------------------------------------------------------------------===//
// Code Generation
//===----------------------------------------------------------------------===//

static std::unique_ptr<LLVMContext> TheContext;
static std::unique_ptr<Module> TheModule;
static std::unique_ptr<IRBuilder<>> Builder;
static std::map<std::string, Value *> NamedValues;

std::unique_ptr<ExprAst> LogErrorA(const char *Str) {
  
    fprintf(stderr, "Error: %s\n", Str);
                            
    return nullptr;

}


Value *LogErrorV(const char *Str) {

    LogErrorA(Str);
    return nullptr;

}



Value *NumberExprAst::codegen() {

    return ConstantFP::get(*TheContext, APFloat(Val));

}



Value *VariableExprAst::codegen() {

    // Look this variable up in the function.
    Value *V = NamedValues[Name];
    
    if (!V)
    
        return LogErrorV("Unknown variable name");
    
    return V;
}

Value *BinaryExprAst::codegen() {

    Value *L = LHS->codegen();
    Value *R = RHS->codegen();
    
    if (!L || !R)
        return nullptr;

    
    switch (Optr) {
    
    case '+':
    
        return Builder->CreateFAdd(L, R, "addtmp");
    
    case '-':
    
        return Builder->CreateFSub(L, R, "subtmp");
    
    case '*':
    
        return Builder->CreateFMul(L, R, "multmp");
    
    case '<':
    
        L = Builder->CreateFCmpULT(L, R, "cmptmp");
        
        // Convert bool 0/1 to double 0.0 or 1.0
        return Builder->CreateUIToFP(L, Type::getDoubleTy(*TheContext), "booltmp");
      
    default:
      
        return LogErrorV("invalid binary operator");
      
      }
}



Value *CallExprAst::codegen() {

    // Look up the name in the global module table.
    Function *CalleeF = TheModule->getFunction(Callee);
    if (!CalleeF)
        return LogErrorV("Unknown function referenced");

    // If argument mismatch error.
    if (CalleeF->arg_size() != Args.size())
        return LogErrorV("Incorrect # arguments passed");

    std::vector<Value *> ArgsV;
    for (unsigned i = 0, e = Args.size(); i != e; ++i) {
        
        ArgsV.push_back(Args[i]->codegen());
        
        if (!ArgsV.back())
          
            return nullptr;
        }

    return Builder->CreateCall(CalleeF, ArgsV, "calltmp");

}



Function *PrototypeAst::codegen() {
    
    // Make the function type:  double(double,double) etc.
    std::vector<Type *> Doubles(Args.size(), Type::getDoubleTy(*TheContext));
    
    FunctionType *FT = FunctionType::get(Type::getDoubleTy(*TheContext), Doubles, false);

    Function *F = Function::Create(FT, Function::ExternalLinkage, Name, TheModule.get());

    // Set names for all arguments.
    unsigned Idx = 0;
    
    for (auto &Arg : F->args())
        Arg.setName(Args[Idx++]);

    return F;

}



Function *FunctionAst::codegen() {
    
    // First, check for an existing function from a previous 'extern' declaration.
    Function *TheFunction = TheModule->getFunction(Proto->getName());

    if (!TheFunction)
        TheFunction = Proto->codegen();

    if (!TheFunction)
        return nullptr;

    // Create a new basic block to start insertion into.
    BasicBlock *BB = BasicBlock::Create(*TheContext, "entry", TheFunction);
    Builder->SetInsertPoint(BB);

    // Record the function arguments in the NamedValues map.
    NamedValues.clear();
    for (auto &Arg : TheFunction->args())
        NamedValues[std::string(Arg.getName())] = &Arg;

    if (Value *RetVal = Body->codegen()) {
        // Finish off the function.
        Builder->CreateRet(RetVal);

        // Validate the generated code, checking for consistency.
        verifyFunction(*TheFunction);

        return TheFunction;
    }

    // Error reading body, remove function.
    TheFunction->eraseFromParent();
    
    return nullptr;
}




















#endif /// LEXER_H






























