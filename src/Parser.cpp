/*================================================================
*    
*   
*   FileName: Parser.cpp
*   Author: DarrenHuang
*   Create Time: 2020/10/20  01:32(Tuesday)
*   Description:
*
================================================================*/

#include "../includes/Parser.h"

#include "../includes/RecursiveDescentParser.h"


Parser::Parser(const vector<shared_ptr<Token>> &tokens):impl(new RecursiveDescentParser(tokens))
{
}

vector<shared_ptr<Stmt>> Parser::parse()
{
    return impl->parse();
}

Parser::~Parser()=default;
