/*================================================================
*    
*   
*   FileName: Token.h
*   Author: DarrenHuang
*   Create Time: 2020/07/06  09:16(星期一)
*   Description:
*       Token  used by Lexer to produce token list
*
================================================================*/
#ifndef _TOKEN_H_
#define _TOKEN_H_



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
