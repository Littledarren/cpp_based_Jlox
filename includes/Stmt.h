/*================================================================
 *    
 *   
 *   FileName: Stmt.h
 *   Author: DarrenHuang
 *   Create Time: 2020/07/04  09:30(星期六)
 *   Description:
 *
 ================================================================*/
#ifndef _STMT_H_
#define _STMT_H_

#include <vector>
using std::vector;

#include "Expr.h"

struct Expression;
struct Print;
struct Var;
struct Block;
struct If;
struct While;

struct Stmt {
    struct Visitor
    {
        virtual void visit(const Expression &stmt)=0;
        virtual void visit(const Print &stmt)= 0;
        virtual void visit(const Var &stmt)=0;
        virtual void visit(const Block &stmt)=0;
        virtual void visit(const If &stmt)=0;
        virtual void visit(const While &stmt)=0;
    };

    virtual void accept(Visitor &visitor)const=0;

#define STMT_VISITABLE() \
    virtual void accept(Visitor &visitor) const override \
    { visitor.visit(*this); }
};

#undef DEBUG
#ifdef DEBUG
#include <iostream>
using std::cout;
using std::endl;
#endif

struct Var : public Stmt
{

    Var(shared_ptr<Token> name, shared_ptr<Expr> initializer):
        name(name), initializer(initializer)
    {
#ifdef DEBUG
        cout<<"=======VAR======="<<endl;
        cout<<name->lexeme<<":::::::::"<<initializer<<endl;
#endif
    }
    STMT_VISITABLE();
    shared_ptr<Token> name;
    shared_ptr<Expr> initializer;

};


struct Expression : public Stmt
{
    Expression(shared_ptr<Expr> expr) : expr(expr){

#ifdef DEBUG
        cout<<"=======EXPR_STMT======="<<endl;
#endif
    }

    STMT_VISITABLE();

    shared_ptr<Expr> expr;
};

struct Print : public Stmt
{
    Print(shared_ptr<Expr> expr) : expr(expr){
#ifdef DEBUG
        cout<<"=======PRINT_STMT======="<<endl;
#endif
    }

    STMT_VISITABLE();
    shared_ptr<Expr> expr;
};

struct Block : public Stmt
{
    Block(const vector<shared_ptr<Stmt>> &statements) : statements(statements){
#ifdef DEBUG
            cout<<"=======BLOCK_STMT======="<<endl;
#endif
    }
    STMT_VISITABLE();

    vector<shared_ptr<Stmt>> statements;
};

struct If : public Stmt
{
    If(shared_ptr<Expr> condition, shared_ptr<Stmt> thenBranch, shared_ptr<Stmt> elseBranch): 
        condition(condition), thenBranch(thenBranch), elseBranch(elseBranch)
    {
#ifdef DEBUG
            cout<<"=======IF_STMT======="<<endl;
#endif
    }
    STMT_VISITABLE();
    shared_ptr<Expr>condition;
    shared_ptr<Stmt> thenBranch;
    shared_ptr<Stmt> elseBranch;
};

struct While : public Stmt
{
    While(shared_ptr<Expr> condition, shared_ptr<Stmt> body): condition(condition), body(body){
#ifdef DEBUG
            cout<<"=======WHILE_STMT======="<<endl;
#endif
    }
    STMT_VISITABLE();

    shared_ptr<Expr>condition;
    shared_ptr<Stmt>body;
};

#endif
