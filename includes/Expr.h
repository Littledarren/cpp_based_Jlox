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
        virtual const Object* visitAssignExpr(const Assign *expr)=0;
        virtual const Object* visitBinaryExpr(const Binary *expr)=0;
        virtual const Object* visitGroupingExpr(const Grouping *expr)=0;
        virtual const Object* visitLiteralExpr(const Literal *expr)=0;
        virtual const Object* visitUnaryExpr(const Unary *expr)=0;
        virtual const Object* visitTernaryExpr(const Ternary *expr)=0;
        virtual const Object* visitLogicalExpr(const Logical *expr)=0;
        virtual const Object* visitVariableExpr(const Variable *expr)=0;
        virtual const Object* visitCallExpr(const Call *expr)=0;
        virtual ~Visitor(){}
    };

    virtual const Object* accept(Visitor *visitor)const = 0;
    virtual ~Expr()
    {}
};

struct Assign : public Expr
{

    Assign(const Token* name, const Expr* value ):name(name), value(value){ }
    const Object* accept(Visitor *visitor) const override 
    {
        return visitor->visitAssignExpr(this);
    }

    ~Assign()
    {
        delete value;
    }

    const Token* name;
    const Expr* value;
};

struct Binary : public Expr
{
    Binary(const Expr *left, const Token *op, const Expr *right):left(left), op(op),right(right)
    {}
    virtual ~Binary()
    {
        delete left;
        //const Token will be freed by Lexer 
        // delete op;
        delete right;
    }
    const Object* accept(Visitor *visitor) const override
    {
        return visitor->visitBinaryExpr(this);
    }
    const Expr* left;
    const Token* op;
    const Expr* right;
};

struct Ternary : public Expr 
{
    Ternary (const Expr *condition, const Expr *if_yes, const Expr *if_no):
        condition(condition), if_yes(if_yes), if_no(if_no)
    {}
    virtual ~Ternary()
    {
        delete condition;
        delete if_yes;
        delete if_no;
    }
    const Object* accept(Visitor *visitor) const override
    {
        return visitor->visitTernaryExpr(this);
    }
    const Expr* condition;
    const Expr* if_yes;
    const Expr* if_no;
};

struct Grouping : public Expr
{
    Grouping(const Expr *expr):expr(expr)
    {}
    virtual ~Grouping()
    {
        delete expr;
    }
    const Object* accept(Visitor *visitor) const override
    {
        return visitor->visitGroupingExpr(this);
    }
    const Expr* expr;
};

#include <cstdio>
//sscanf lives forever!!!!!!!!
struct Literal: public Expr
{
    Literal(const Object* value=nullptr):value(value)
    {}
    string getStr()
    {
        if (value == nullptr) return "NULLLLLLLL pointer ERROR";
        return value->toString();
    }
    virtual ~Literal()
    {
    }
    const Object* accept(Visitor *visitor) const override
    {
        return visitor->visitLiteralExpr(this);
    }
    const Object *value;
};

struct Unary : public Expr
{
    Unary(const Token *op, const Expr *right) : op(op), right(right)
    {}
    virtual ~Unary()
    {
        //delete op;
        delete right;
    }
    const Object* accept(Visitor *visitor) const override
    {
        return visitor->visitUnaryExpr(this);
    }
    const Token* op;
    const Expr* right;
};
struct Variable : public Expr
{
    Variable(const Token* name):
        name(name){}
    const Object* accept(Visitor *visitor) const override
    {
        return visitor->visitVariableExpr(this);
    }
    const Token * name;
};

struct Logical : public Expr 
{
    Logical(const Expr* left, const Token* op, const Expr* right):
        left(left), op(op), right(right){}
    const Object* accept(Visitor *visitor) const override
    {
        return visitor->visitLogicalExpr(this);
    }
    ~Logical()
    {
        delete left;
        delete right;
    }

    const Expr *left;
    const Token *op;
    const Expr *right;
};

struct Call : public Expr
{
    Call(const Expr *callee, const Token *paren, const vector<Expr*> &arguments):
        callee(callee), paren(paren), arguments(arguments){}
    ~Call()
    {
        delete callee;
        for (auto &p : arguments) 
            delete p;
    }
    const Object* accept(Visitor *visitor) const override
    {
        return visitor->visitCallExpr(this);
    }
    const Expr *callee;
    const Token *paren;
    vector<Expr*> arguments;
};
#endif
