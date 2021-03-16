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



//#define RETURE_TYPE shared_ptr<const Object>

using RETURN_TYPE = shared_ptr<Object>;

struct Assign;
struct Binary;
struct Unary;
struct Grouping;
struct Literal;
struct Ternary;
struct Variable;
struct Logical;
struct Call;
struct Lambda;

struct Expr
{
    struct Visitor
    {
        virtual RETURN_TYPE visit(const Assign &expr)=0;
        virtual RETURN_TYPE visit(const Binary &expr)=0;
        virtual RETURN_TYPE visit(const Grouping &expr)=0;
        virtual RETURN_TYPE visit(const Literal &expr)=0;
        virtual RETURN_TYPE visit(const Unary &expr)=0;
        virtual RETURN_TYPE visit(const Ternary &expr)=0;
        virtual RETURN_TYPE visit(const Logical &expr)=0;
        virtual RETURN_TYPE visit(const Variable &expr)=0;
        virtual RETURN_TYPE visit(const Call &expr)=0;
        virtual RETURN_TYPE visit(const Lambda &expr)=0;
        
    };

    virtual RETURN_TYPE accept(Visitor &visitor)const = 0;
#define EXPR_VISITABLE() \
    RETURN_TYPE accept(Visitor &visitor) const override \
    { return visitor.visit(*this); }
};

#undef DEBUG
#ifdef DEBUG
using std::cout;
using std::endl;
#endif

struct Assign : public Expr
{

    Assign(shared_ptr<Token> name, shared_ptr<Expr> value):name(name), value(value)
    {

#ifdef DEBUG
        cout<<"============ASSIGN_EXPR=============="<<endl;
#endif
    }

    EXPR_VISITABLE()

    shared_ptr<Token> name;
    shared_ptr<Expr> value;
};

struct Binary : public Expr
{
    Binary(shared_ptr<Expr>left, shared_ptr<Token>op, shared_ptr<Expr>right):left(left), op(op),right(right)
    {
        #ifdef DEBUG
        cout<<"============Binary_EXPR=============="<<endl;
#endif
    }
    EXPR_VISITABLE();
    shared_ptr< Expr> left;
    shared_ptr< Token> op;
    shared_ptr< Expr> right;
};

struct Ternary : public Expr 
{
    Ternary (shared_ptr<Expr>condition, shared_ptr<Expr>if_yes, shared_ptr<Expr>if_no):
        condition(condition), if_yes(if_yes), if_no(if_no)
    {
        #ifdef DEBUG
        cout<<"============Ternary_EXPR=============="<<endl;
#endif
    }
    EXPR_VISITABLE();

    shared_ptr<Expr> condition;
    shared_ptr<Expr> if_yes;
    shared_ptr<Expr> if_no;
};

struct Grouping : public Expr
{
    Grouping(shared_ptr<Expr>expr):expr(expr)
    {
        #ifdef DEBUG
        cout<<"============Grouping_EXPR=============="<<endl;
#endif
    }
    EXPR_VISITABLE();
    shared_ptr<Expr> expr;
};

struct Literal: public Expr
{
    explicit Literal(RETURN_TYPE value=nullptr):value(value)
    {
#ifdef DEBUG
        cout<<"============Literal_EXPR=============="<<endl;
#endif
    }
    string getStr()
    {
        if (!value) return "NIL";
        return value->toString();
    }
    EXPR_VISITABLE();

    // string, numbers, bool, Nil
    RETURN_TYPE value;
};

struct Unary : public Expr
{
    Unary(shared_ptr<Token>op, shared_ptr<Expr>right) : op(op), right(right)
    {
#ifdef DEBUG
        cout<<"============Unary_EXPR=============="<<endl;
#endif
    }
    EXPR_VISITABLE();
    shared_ptr<Token> op;
    shared_ptr<Expr> right;
};
struct Variable : public Expr
{
    Variable(shared_ptr<Token> name):
        name(name)
    {
#ifdef DEBUG
        cout<<"============Variable_EXPR=============="<<endl;
#endif
    }
    EXPR_VISITABLE();
    shared_ptr<Token> name;
};

struct Logical : public Expr 
{
    Logical(shared_ptr<Expr> left, shared_ptr<Token> op, shared_ptr<Expr> right):
        left(left), op(op), right(right)
    {
#ifdef DEBUG
        cout<<"============Logical_EXPR=============="<<endl;
#endif
    }

    EXPR_VISITABLE();
    shared_ptr<Expr>left;
    shared_ptr<Token>op;
    shared_ptr<Expr>right;
};

struct Call : public Expr
{
    Call(shared_ptr<Expr>callee, shared_ptr<Token>paren, vector<shared_ptr<Expr>> &arguments):
        callee(callee), paren(paren), arguments(arguments)
    {
#ifdef DEBUG
        cout<<"============Call_EXPR=============="<<endl;
#endif
    }
    EXPR_VISITABLE();
    shared_ptr<Expr>callee;
    shared_ptr<Token>paren;
    vector<shared_ptr<Expr>> arguments;
};
struct Stmt;
struct Lambda : public Expr
{
    Lambda(vector<shared_ptr<Token>> params,vector<shared_ptr<Stmt>> body):
        params(params), body(body)
    {
#ifdef DEBUG
            cout<<"=======Lambda_expr======="<<endl;
#endif

    }
    
    EXPR_VISITABLE();
    vector<shared_ptr<Token>> params;
    vector<shared_ptr<Stmt>> body;
};
#endif
