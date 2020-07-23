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
#include "Stmt.h"

class Parser

{
public:
    Parser(const vector<const Token*> &tokens):tokens(tokens)
    {}

    virtual vector<Stmt*> parse()=0;
    virtual ~Parser(){}

protected:
   const vector<const Token*> &tokens;

};


#endif
