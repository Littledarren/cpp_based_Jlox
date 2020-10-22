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
#include <vector>
#include "Token.h"

using std::string;
using std::vector;

struct Assign;
struct Binary;
struct Unary;
struct Grouping;
struct Literal;
struct Ternary;
struct Variable;
struct Logical;
struct Call;

struct Expr
{
    struct Visitor
    {
        virtual shared_ptr<const Object> visitAssignExpr(const Assign *expr)=0;
        virtual shared_ptr<const Object> visitBinaryExpr(const Binary *expr)=0;
        virtual shared_ptr<const Object> visitGroupingExpr(const Grouping *expr)=0;
        virtual shared_ptr<const Object> visitLiteralExpr(const Literal *expr)=0;
        virtual shared_ptr<const Object> visitUnaryExpr(const Unary *expr)=0;
        virtual shared_ptr<const Object> visitTernaryExpr(const Ternary *expr)=0;
        virtual shared_ptr<const Object> visitLogicalExpr(const Logical *expr)=0;
        virtual shared_ptr<const Object> visitVariableExpr(const Variable *expr)=0;
        virtual shared_ptr<const Object> visitCallExpr(const Call *expr)=0;
        virtual ~Visitor(){}
    };

    virtual shared_ptr<const Object> accept(Visitor *visitor)const = 0;
    virtual ~Expr()
    {}
};

struct Assign : public Expr
{

    Assign(shared_ptr<const Token> name, shared_ptr<const Expr> value):name(name), value(value){ }
    shared_ptr<const Object> accept(Visitor *visitor) const override 
    {
        return visitor->visitAssignExpr(this);
    }

    shared_ptr<const Token> name;
    shared_ptr<const Expr> value;
};

struct Binary : public Expr
{
    Binary(shared_ptr<const Expr>left, shared_ptr<const Token>op, shared_ptr<const Expr>right):left(left), op(op),right(right)
    {}
    shared_ptr<const Object> accept(Visitor *visitor) const override
    {
        return visitor->visitBinaryExpr(this);
    }
    shared_ptr<const Expr> left;
    shared_ptr<const Token> op;
    shared_ptr<const Expr> right;
};

struct Ternary : public Expr 
{
    Ternary (shared_ptr<const Expr>condition, shared_ptr<const Expr>if_yes, shared_ptr<const Expr>if_no):
        condition(condition), if_yes(if_yes), if_no(if_no)
    {}
    shared_ptr<const Object> accept(Visitor *visitor) const override
    {
        return visitor->visitTernaryExpr(this);
    }
    shared_ptr<const Expr> condition;
    shared_ptr<const Expr> if_yes;
    shared_ptr<const Expr> if_no;
};

struct Grouping : public Expr
{
    Grouping(shared_ptr<const Expr>expr):expr(expr)
    {}
    shared_ptr<const Object> accept(Visitor *visitor) const override
    {
        return visitor->visitGroupingExpr(this);
    }
    shared_ptr<const Expr> expr;
};

#include <cstdio>
//sscanf lives forever!!!!!!!!
struct Literal: public Expr
{
    Literal(shared_ptr<const Object> value=nullptr):value(value)
    {}
    string getStr()
    {
        if (!value) return "NULLLLLLLL pointer ERROR";
        return value->toString();
    }
    shared_ptr<const Object> accept(Visitor *visitor) const override
    {
        return visitor->visitLiteralExpr(this);
    }
    shared_ptr<const Object>value;
};

struct Unary : public Expr
{
    Unary(shared_ptr<const Token>op, shared_ptr<const Expr>right) : op(op), right(right)
    {}
    shared_ptr<const Object> accept(Visitor *visitor) const override
    {
        return visitor->visitUnaryExpr(this);
    }
    shared_ptr<const Token> op;
    shared_ptr<const Expr> right;
};
struct Variable : public Expr
{
    Variable(shared_ptr<const Token> name):
        name(name){}
    shared_ptr<const Object> accept(Visitor *visitor) const override
    {
        return visitor->visitVariableExpr(this);
    }
    shared_ptr<const Token> name;
};

struct Logical : public Expr 
{
    Logical(shared_ptr<const Expr> left, shared_ptr<const Token> op, shared_ptr<const Expr> right):
        left(left), op(op), right(right){}
    shared_ptr<const Object> accept(Visitor *visitor) const override
    {
        return visitor->visitLogicalExpr(this);
    }

    shared_ptr<const Expr>left;
    shared_ptr<const Token>op;
    shared_ptr<const Expr>right;
};

struct Call : public Expr
{
    Call(shared_ptr<const Expr>callee, shared_ptr<const Token>paren, const vector<shared_ptr<Expr>> &arguments):
        callee(callee), paren(paren), arguments(arguments){}
    shared_ptr<const Object> accept(Visitor *visitor) const override
    {
        return visitor->visitCallExpr(this);
    }
    shared_ptr<const Expr>callee;
    shared_ptr<const Token>paren;
    vector<shared_ptr<Expr>> arguments;
};
#endif
