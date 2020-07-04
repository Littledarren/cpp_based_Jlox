#ifndef MAIN_H
#define MAIN_H
#include <iostream>

using namespace std;
extern bool hadError; //has error ?
extern bool hadRuntimeError;

#include "Token.h"
#include "Interpreter.h"

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
    cerr<<e.what()<<"\n[line " + getNameOfType(e.type) +"]";
    hadRuntimeError = true;

}




#endif

