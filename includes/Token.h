#ifndef TOKEN_H
#define TOKEN_H

#include "TokenType.h"

class Token 
{
public:
    Token(TokenType type, const string &lexeme, void * literal, int line):
        type(type), lexeme(lexeme), literal(literal), line(line)
    {
    };
    operator string()
    {
        return getNameOfType(type) + " " + lexeme + " ";
    }
    virtual ~Token()
    {
        //will be freed by Lexer.
        switch(type) {
            case STRING:
                delete (string*)literal;
                break;
            case NUMBER:
                delete (double*)literal;
                break;
            default:
                //nothing to do with literal
                break;
        }
    }
//private:
    const TokenType type;
    const string lexeme;
    void * const literal;
    const int line;
};

#endif
