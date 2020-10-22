/*================================================================
*    
*   
*   FileName: Scanner.h
*   Author: DarrenHuang
*   Create Time: 2020/06/14  18:59(Sunday)
*   Description:
*
================================================================*/
#ifndef _SCANNER_H_
#define _SCANNER_H_

#include "Lexer.h"
#include "Token.h"
#include "main.h"

#include <sstream>
#include <map>

class Scanner 
{
public:
    const vector<shared_ptr<Token>>& scanTokens();
    Scanner(const string &source):source(source){}
private:
    bool isAtEnd();
    void scanToken();
    void takeIdentifier();
    bool isAlpha(char c);
    bool isAlphaNumeric(char c);
    bool isDigit(char c);
    void takeNumber();
    char peekNext();
    void takeString();
    char peek(); 
    bool match(char expected);
    char advance();
    void addToken(TokenType type);
    void addToken(TokenType type, shared_ptr<const Object> literal);

    //new feature! after..c++11?
    const string &source;
    vector<shared_ptr<Token>> tokens_list;
    int start = 0;
    int current = 0;
    int line = 1;
    static std::map<string, TokenType> keywords;
};

#endif
