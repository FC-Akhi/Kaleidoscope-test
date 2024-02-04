//===- Parser.h - Kelidoscop Language Parser -------------------------------------===//
//
// This file implements the parser for the Kelidoscop language. It processes the Token
// provided by the Lexer and returns an AST.
//
//===----------------------------------------------------------------------===//



/// Prevents the AST header from being included multiple times
/// which can lead to definition conflicts.
#ifndef AST_H
#define AST_H



#include "AST.h"


#include <vector>
#include <optional>
#include <iostream>



/// Global variable to hold the current token. This is used across the parser to determine the next course of action based on the current token's value.
static int CurTok; 


/// Fetches the next token from the lexer and updates CurTok. Prints the current and next token for debugging purposes.
static int getNextToken() {

    printf("CurTok: %d\n", CurTok);
    
    CurTok = getTok();

    printf("NextTok: %d\n", CurTok);

    return CurTok;

}



/// LogError* - These are little helper functions for error handling.
std::unique_ptr<ExprAst> LogError(const char *Str) {
  
    fprintf(stderr, "Error: %s\n", Str);
                            
    return nullptr;

}

std::unique_ptr<PrototypeAst> LogErrorP(const char *Str) {

    LogError(Str);

    return nullptr;

}



/// Parses numeric expressions. It creates a NumberExprAst with the current numeric value,
/// prints the result for debugging, and advances to the next token.
/// numberexpr ::= number
static std::unique_ptr<ExprAst> ParseNumberExpr() {

    printf("Inside ParseNumberExpr\n");

    auto Result = std::make_unique<NumberExprAst>(NumVal);

    std::cout << "Result: " << Result->getVal() << std::endl;

    getNextToken(); /// eat: move the pointer to the next character

    return std::move(Result);

}



/// identifierexpr ::= identifier
static std::unique_ptr<ExprAst> ParseIdentifierExpr() {

    std::string IdName = IdentifierStr;

    getNextToken();

    if (CurTok != '(') { /// If its only identifier, Eg: name. But not identifier with parameter surrounded by parenthesis. Eg: foo(int x) 

        return std::make_unique<VariableExprAst>(IdName);
    
    } else {

        return nullptr;

    }   

}




/// Decision point for parsing primary expressions based on the current token.
/// It can handle numeric and variable for now.
/// primary ::= numberexpr | identifierexpr 
static std::unique_ptr<ExprAst> ParsePrimary() {

    switch(CurTok) {


    case TK_number:

        printf("Inside ParsePrimary for case TK_number\n");

        return ParseNumberExpr();

    case TK_identifier:

        printf("Inside ParsePrimary for case TK_identifier\n");

        return ParseIdentifierOrCallExpr();

    default:

        return LogError("unknown token when expecting an expression");


    }
}




/// Determines the precedence of the current token. 
/// This is used to parse binary operation expressions correctly, adhering to operator precedence rules.
static int GetTokPrecedence() {

    switch (CurTok) {

    case '<':
    case '>':

        return  10;

    case '+':
    case '-':

        return 20;

    case '*':
    case '/':

        return 30;

    default:

        return -1; /// Indicates an unknown operator or no precedence.

    }
}



/// For parsing the right-hand side of binary operations of an expression.
/// Currently, it simply returns the left-hand side as is, without further parsing.
/// binoprhs ::= ('+' | '-' | '*' | '/' | '<' | '>' primary)*
static std::unique_ptr<ExprAst> ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAst> LHS) { /// ExprPrec hold precedence number. 
    
    printf("Inside ParseBinOpRHS\n");

    while (true) {

        int TokPrec = GetTokPrecedence();

        if (TokPrec < ExprPrec) { /// This is the part for only a variable or a number

            return LHS;

        } else { /// This is the part for when it is an expression with binary operation, not only a variable or a number

            return nullptr; /// For now we will skip this part
        }

    }
}



/// Parses expressions, which can be simple expression with only number or a vairbale or binary operations.
/// expression ::= primary binoprhs
static std::unique_ptr<ExprAst> ParseExpression() {

    printf("Inside ParseExpression\n");

    auto LHS = ParsePrimary();

    std::cout << "LHS: " << LHS->getVal() << std::endl;

    /// If we are able to parse it via ParsePrimary()
    if(LHS) {

        return ParseBinOpRHS(0, std::move(LHS));

    }

    return nullptr;

}



/// Parses top-level expressions, wrapping them into an anonymous function construct.
/// toplevelexpr ::= expression
static std::unique_ptr<FunctionAst> ParseTopLevelExpr() {

    printf("Inside ParseTopLevelExpr\n");

    auto E = ParseExpression();

    if (E) {

        /// Make an anonymous proto
        auto Proto = std::make_unique<PrototypeAst>("", std::vector<std::string>());

        return std::make_unique<FunctionAst>(std::move(Proto), std::move(E));

    } else {

        return nullptr;

    }
}




///===-----------------------------------------------------------------------------
/// Top-Level parsing
///===-----------------------------------------------------------------------------



/// Handles expressions at the top level, attempting to parse them and logging success or moving to the next token for recovery on failure.
static void HandleTopLevelExpression() {

    printf("Inside HandleTopLevelExpression\n");

    /// Evaluate top level expression in to anonymous function
    if (ParseTopLevelExpr()) {

        fprintf(stderr, "Parsed TopLevelExpr\n");
    
    } else {

        /// Skip token for error recovery
        getNextToken();

    }

}




// Main loop for the parser. It waits for input and directs the parser based on the type of the current token.
/// top ::= expression
static void MainLoop() {

    printf("Inside MainLoop\n");

    while (true) {

        fprintf(stderr, "ready> ");

        switch (CurTok) {

        case TK_eof: /// ignore top-level eof for now

            return;

        case ';': /// ignore top-level semicolons for now

            getNextToken();
            break;


        default:

            HandleTopLevelExpression();
            break;

        }
    }
}




#endif // AST_H



















































































