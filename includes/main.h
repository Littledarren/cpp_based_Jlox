#ifndef MAIN_H
#define MAIN_H
#include <iostream>

using namespace std;
extern bool hadError; //has error ?

#include "Token.h"

inline void report(int line, const string &where, const string &message)
{
    cout<<"[line "<<line<<"] Error "<<where<<" : "<<message<<endl;
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




#endif

