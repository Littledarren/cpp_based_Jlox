/*================================================================
*    
*   
*   FileName: Lexer.cpp
*   Author: DarrenHuang
*   Create Time: 2020/10/19  23:58(Monday)
*   Description:
*
================================================================*/


#include "../includes/Lexer.h"


//implementation
#include "../includes/Scanner.h"


const vector<shared_ptr<Token>>& Lexer::scanTokens()
{
    return impl->scanTokens();
}

Lexer::Lexer(const string &source):impl(new Scanner(source))
{
}

Lexer::~Lexer()=default;
