#ifndef LEXER_H
#define LEXER_H
#include <vector>
#include <memory>
using std::vector;
using std::unique_ptr;
using std::shared_ptr;

#include "Token.h"

class Scanner;
//interface for lexical parsing
class Lexer
{
public:
    const vector<shared_ptr<Token>>& scanTokens();
    Lexer(const string &source);
    ~Lexer();
private:
    unique_ptr<Scanner> impl;
};

#endif
