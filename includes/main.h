/*================================================================
*    
*   
*   FileName: main.h
*   Author: DarrenHuang
*   Create Time: 2020/10/19  19:48(Monday)
*   Description:
*
================================================================*/

#ifndef MAIN_H
#define MAIN_H
#include <iostream>

#include "Token.h"
#include "RuntimeError.h"

using std::cout;
using std::cin;
using std::cerr;
using std::endl;

extern bool hadError; //has error ?
extern bool hadRuntimeError;

inline void report(int line, const string &where, const string &message)
{
    cerr<<"[line "<<line<<"] Error "<<where<<" : "<<message<<endl;
    hadError = true;
}
inline void error(int line, const string &message) 
{
    report(line, "", message);
}
inline void error(const Token &token, const string &msg)
{
    if (token.type == FOE) {
        report(token.line, "at end", msg);
    } else {
        report(token.line, "at '" + token.lexeme +"'", msg);
    }
}

inline void runtimeError(const RuntimeError &e)
{
    cerr<<e.what()<<"\n[line "<<  e.token->line <<"]";
    hadRuntimeError = true;
}

#endif

