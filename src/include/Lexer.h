//===- Lexer.h - Lexer for the Kelidoscop language -------------------------------===//
//
// This file implements a simple Lexer for the Kelidoscop language.
//
//===----------------------------------------------------------------------===//

#include <stdio.h>
#include <string>



// List of Token returned by the lexer.
// enum represents integer. If we not declare the type (int), it also will work
// If we not assign any int values o the token, 
// enum will aautomatically assign values starting from 0. (Like: tok_eof = 0, tok_def = 1 and so on.)
// But, we explicitly assigned negative values which is not used in ascii table and have clear separation
// that negatives representing tokens not anything from ascii table. And ascii already taken [0-255]
// The lexer returns tokens [0-255] if it is an unknown character, otherwise return one
// of these number from below list for known things.
enum TokenKind : int {

    TK_eof = -1,

    // commands
    TK_def = -2,
    TK_extern = -3,

    // primary
    TK_identifier = -4,
    TK_number = -5,
};



// Define global variable
static std::string IdentifierStr; // Whenever lexer crawl the code and identify identifier it will store in this variable
static double NumVal;             // Whenever lexer crawl the code and identify number it will store in this variable



// The actual implementation of the lexer is a single function named getTok. 
// Read a single character and avoid whitespaces
static int getTok() {

    static int lastChar = ' ';

    // Skip whitespaces
    while (isspace(lastChar)) {

        lastChar = getchar();

        printf("lastChar: %d\n", lastChar);
    }


    // Get identifier
    if (isalpha(lastChar)) { // return nonzero if lastChar is alphabet


        IdentifierStr = lastChar; 

        printf("1. IdentifierStr: %s\n", IdentifierStr.c_str());

        while (isalnum ((lastChar = getchar()))) { 

            IdentifierStr += lastChar;

            printf("2. IdentifierStr: %s\n", IdentifierStr.c_str());
            
        }

        printf("3. IdentifierStr: %s\n", IdentifierStr.c_str());

        if (IdentifierStr == "def") {

            return TK_def;

        }


        if (IdentifierStr == "extern") {

            return TK_extern;

        }

        return TK_identifier;


    }


    // Get number
    if (isdigit(lastChar) || lastChar == '.') {


        std::string numStr;

        numStr = lastChar;  

        printf("1. numStr: %s\n", numStr.c_str());

        do {


            lastChar = getchar();

            numStr += lastChar;

            printf("2. numStr: %s\n", numStr.c_str());

        } while (isdigit(lastChar) || lastChar == '.');

        printf("3. numStr: %s\n", numStr.c_str());

        NumVal = strtod(numStr.c_str(), 0); // converts to double type

        return TK_number;


    }


    // Ignore comments
    if (lastChar == '#') {

        do {

            lastChar = getchar();

        } while (lastChar != EOF && lastChar != '\n' && lastChar != '\r');


        if (lastChar != EOF) {

            return getTok();

        }

    }


    // Get EOF
    if (lastChar == EOF) {

        return TK_eof;
    
    }


    // For other undefined characters
    int thisChar = lastChar;

    lastChar = getchar();
    
    return thisChar;


}

