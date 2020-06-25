#ifndef LEXER_H
#define LEXER_H
#include <vector>
using std::vector;
#include "Token.h"

//abstract class for lexical parsing
class Lexer
{
public:
    virtual vector<Token*> scanTokens()=0;
    Lexer(const string &source):
        source(source), tokens()
    {
    };
    virtual ~Lexer()
    {
        //delete tokens
        for (auto p : tokens) {
            delete p;
        }
    };
protected:
    const string source;
    vector<Token*> tokens;
};

#endif
