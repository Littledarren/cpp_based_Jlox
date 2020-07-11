/*================================================================
*    
*   
*   FileName: RecursiveDescentParser.h
*   Author: DarrenHuang
*   Create Time: 2020/06/19  11:00(Friday)
*   Description:
*
================================================================*/
#ifndef _RECURSIVEDESCENTPARSER_H_
#define _RECURSIVEDESCENTPARSER_H_

#include <initializer_list>
#include <exception>
#include "Parser.h"
#include "Expr.h"
#include "main.h"

class ParseError : public std::runtime_error
{
public:
    ParseError(const string &s):
        runtime_error(s)
    {
    }
};

// a implementation of Parser
class RecursiveDescentParser : public Parser
{
public:

    RecursiveDescentParser(vector<Token*> &tokens) : Parser(tokens)
    {}
    virtual vector<Stmt*> parse() override;
private:
    //声明，后面会包括函数声明，类声明
    //现在只有变量定义
    Stmt* declaration();
    //用来解析变量定义
    Stmt* varDeclaration();
    //语句，即可执行的语句.
    Stmt* statement();
    Stmt* printStatement();
    Stmt* expressionStatement();
    vector<Stmt*> block();

    Expr* expression();

    Expr* assignment();
    Expr* commaExpression();
    Expr* ternaryExpression();
    Expr* equality();
    Expr* comparison();
    Expr* addition();
    Expr* multiplication();
    Expr* unary();
    Expr* primary();

    Token* consume(TokenType type,const string &message);
    ParseError error(Token *token, const string &message);
    void synchronize();

    bool match(std::initializer_list<TokenType> types)
    {
        //toleratable types list
        for (auto t : types) {
            if (check(t)) {
                advance();
                return true;
            }
        }
        return false;

    }
    bool check(TokenType t) const
    {
        if (isAtEnd()) return false;
        return peek()->type == t;
    }
    Token* advance()
    {
        if (!isAtEnd()) current++;
        return previous();
    }

    bool isAtEnd() const
    {
        return peek()->type == FOE;
    }

    Token* peek() const
    {
        return tokens.at(current);
    }
    Token* previous() const
    {
        return tokens.at(current-1);
    }

    int current = 0;

};

#endif
