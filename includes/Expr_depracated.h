
#ifndef _EXPR_H_
#define _EXPR_H_

#include <iostream>
#include "TokenType.h"
#include "Token.h"
using namespace std;
template<typename R> struct Visitor;
template<typename R> struct Binary;
template<typename R>
struct Expr
{
    virtual R  accept(Visitor<R> visitor)=0;
};
template<typename R>
struct Visitor {
    virtual R visitBinaryExpr(Binary<R> expr)=0;
    virtual R visitGroupingExpr(Grouping expr)=0;
    virtual R visitLiteralExpr(Literal expr)=0;
    virtual R visitUnaryExpr(Unary expr)=0; 
};

template<typename R>
struct Binary:public Expr<R>
{
    Binary(Expr<R> *left, Token *op, Expr<R> *right):left(left),op(op),right(right)
                                               
    {}
    ~Binary()
    {
        delete left;
        delete op;
        delete right;

    }
    R accept(Visitor<R> visitor) override {return visitor.visitBinaryExpr(this);
    }
    const Expr<R> *left;
    const Token *op;
    const Expr<R> *right;
};
struct Grouping:public Expr
{
    Grouping(Expr *expression):expression(expression)
    {}
    ~Grouping()
    {
        delete expression;

    }
    template<typename R>
        R accept(Visitor<R> visitor) override {return visitor.visitGroupingExpr(this);
        }
    const Expr *expression;
};
struct Literal:public Expr
{
    Literal(TokenType *type, void *value):type(type),value(value)
    {}
    ~Literal()
    {

        switch(*type)
        {
            case STRING:
                delete (string*)value;
                break;
            case NUMBER:
                delete (double*)value;
                break;
            default:
                std::cout<<"RUNTIME ERROR WHEN DELETING " <<getNameOfType(*type)<<std::endl;
        }
        delete type;
    }
    template<typename R>
        R accept(Visitor<R> visitor) override {return visitor.visitLiteralExpr(this);
        }
    const TokenType *type;
    const void *value;
};
struct Unary:public Expr
{
    Unary(Token *op, Expr *right):op(op),right(right)
    {}
    ~Unary()
    {
        delete op;
        delete right;

    }
    template<typename R>
        R accept(Visitor<R> visitor) override {return visitor.visitUnaryExpr(this);
        }
    const Token *op;
    const Expr *right;
};

#endif
