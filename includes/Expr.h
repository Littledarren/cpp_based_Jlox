/*================================================================
 *    
 *   
 *   FileName: Expr.h
 *   Author: DarrenHuang
 *   Create Time: 2020/06/17  09:49(Wednesday)
 *   Description:
 *      Abstract Syntax Tree
 *
 ================================================================*/
#ifndef _EXPR_H_
#define _EXPR_H_

#include <iostream>
#include "TokenType.h"
#include "Token.h"

using std::string;

struct Binary;
struct Unary;
struct Grouping;
struct Literal;
struct Ternary;

struct Visitor;

struct Expr
{
    virtual void* accept(Visitor *visitor) = 0;
    virtual ~Expr()
    {}
};

struct Visitor
{
    virtual void* visitBinaryExpr(Binary *expr)=0;
    virtual void* visitGroupingExpr(Grouping *expr)=0;
    virtual void* visitLiteralExpr(Literal *expr)=0;
    virtual void* visitUnaryExpr(Unary *expr)=0;
    virtual void* visitTernaryExpr(Ternary *expr)=0;
    virtual ~Visitor(){}

};
struct Binary : public Expr
{
    Binary(Expr *left, Token *op, Expr *right):left(left), op(op),right(right)
    {}
    virtual ~Binary()
    {
        delete left;
        //token will be freed by Lexer 
        // delete op;
        delete right;
    }
    void* accept(Visitor *visitor) override
    {
        return visitor->visitBinaryExpr(this);
    }
    Expr *const left;
    Token *const op;
    Expr *const right;
};

struct Ternary : public Expr 
{
    Ternary (Expr *condition, Expr *if_yes, Expr *if_no):
        condition(condition), if_yes(if_yes), if_no(if_no)
    {}
    virtual ~Ternary()
    {
        delete condition;
        delete if_yes;
        delete if_no;
    }
    void* accept(Visitor *visitor) override
    {
        return visitor->visitTernaryExpr(this);
    }
    Expr *const condition;
    Expr *const if_yes;
    Expr *const if_no;
};

struct Grouping : public Expr
{
    Grouping(Expr *expr):expr(expr)
    {}
    virtual ~Grouping()
    {
        delete expr;
    }
    void* accept(Visitor *visitor) override
    {
        return visitor->visitGroupingExpr(this);
    }
    Expr *const expr;
};

#include <cstdio>
//sscanf lives forever!!!!!!!!
struct Literal: public Expr
{
    Literal(TokenType *type, void *value):type(type), value(value)
    {}
    string getStr()
    {
        return *(string*)value;
    }
    double getDouble()
    {
        return *(double*)value;
    }
    //result should be deleted by caller
    //because some returned pointer will not be hold by Literal
    string* getValStr()
    {
        switch(*type)
        {
            case STRING:
                return new string(*(string*)value);
                break;
            case NUMBER:
                {

                    char buf[256];
                    double val = *(double*)value;
                    sprintf(buf, "%f", val);
                    return new string(buf);
                    break;
                }
            case TRUE:
                return new string("true");
                break;
            case FALSE:
                return new string("false");
                break;
            default:
                throw "RUNTIME ERROR WHEN DELETING ";
        }
    }

    virtual ~Literal()
    {
        delete type;
        //value wille be freed by Token
    }
    void* accept(Visitor *visitor) override
    {
        return visitor->visitLiteralExpr(this);
    }
    TokenType *const type;
    void *const value;
};

struct Unary : public Expr
{
    Unary(Token *op, Expr *right) : op(op), right(right)
    {}
    virtual ~Unary()
    {
        //delete op;
        delete right;
    }
    void* accept(Visitor *visitor) override
    {
        return visitor->visitUnaryExpr(this);
    }
    Token *const op;
    Expr *const right;
};

#endif
