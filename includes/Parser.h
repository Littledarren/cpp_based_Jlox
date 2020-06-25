/*================================================================
*    
*   
*   FileName: Parser.h
*   Author: DarrenHuang
*   Create Time: 2020/06/19  10:53(Friday)
*   Description:
*
================================================================*/
#ifndef _PARSER_H_
#define _PARSER_H_

#include <vector>
using std::vector;

#include "TokenType.h"
#include "Token.h"
#include "Expr.h"

class Parser

{
public:
    Parser(vector<Token*> &tokens):tokens(tokens)
    {}

    virtual Expr* parse()=0;
    virtual ~Parser(){}


protected:
   vector<Token*> &tokens;

};


#endif
