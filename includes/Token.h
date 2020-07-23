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
#include "Value.h"

struct Token 
{
public:
    Token(TokenType type, const string &lexeme,const Object *literal, int line):
        type(type), lexeme(lexeme), literal(literal), line(line)
    {
    };
    operator string() const
    {
        return getNameOfType(type) + " " + lexeme + " ";
    }
    virtual ~Token()
    {
        delete literal;
    }
//private:
    const TokenType type;
    const string lexeme;
    const Object* const literal;
    const int line;
};

#endif
