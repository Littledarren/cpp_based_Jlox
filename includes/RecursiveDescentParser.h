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
class RecursiveDescentParser 
{
public:

    RecursiveDescentParser(const vector<shared_ptr<Token>> &tokens) : tokens(tokens)
    {}
    vector<shared_ptr<Stmt>> parse() ;
private:
    //声明，后面会包括函数声明，类声明
    //现在只有变量定义
    shared_ptr<Stmt> declaration();
    //用来解析变量定义
    shared_ptr<Stmt> varDeclaration();
    //语句，即可执行的语句.
    shared_ptr<Stmt> statement();
    shared_ptr<Stmt> ifStatement();
    shared_ptr<Stmt> whileStatement();
    shared_ptr<Stmt> forStatement();
    shared_ptr<Stmt> printStatement();
    shared_ptr<Stmt> expressionStatement();
    vector<shared_ptr<Stmt>> block();

    shared_ptr<Expr> expression();

    shared_ptr<Expr> assignment();
    shared_ptr<Expr> logicalOr();
    shared_ptr<Expr> logicalAnd();

    shared_ptr<Expr> commaExpression();
    shared_ptr<Expr> ternaryExpression();
    shared_ptr<Expr> equality();
    shared_ptr<Expr> comparison();
    shared_ptr<Expr> addition();
    shared_ptr<Expr> multiplication();
    shared_ptr<Expr> unary();
    shared_ptr<Expr> call();
    shared_ptr<Expr> finishCall(shared_ptr<Expr>callee_test);
    shared_ptr<Expr> primary();

    shared_ptr<const Token> consume(TokenType type,const string &message);
    ParseError error(shared_ptr<const Token>token, const string &message);
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
    shared_ptr<const Token> advance()
    {
        if (!isAtEnd()) current++;
        return previous();
    }

    bool isAtEnd() const
    {
        return peek()->type == FOE;
    }

    shared_ptr<const Token> peek() const
    {
        return tokens.at(current);
    }
    shared_ptr<const Token> previous() const
    {
        return tokens.at(current-1);
    }
    const vector<shared_ptr<Token>> &tokens;
    int current = 0;

};

#endif
