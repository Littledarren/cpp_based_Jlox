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
        virtual void visitExpressionStmt(const Expression *stmt)=0;
        virtual void visitPrintStmt(const Print *stmt)= 0;
        virtual void visitVarStmt(const Var *stmt)=0;
        virtual void visitBlockStmt(const Block *stmt)=0;
        virtual void visitIfStmt(const If *stmt)=0;
        virtual void visitWhileStmt(const While *stmt)=0;
        virtual ~Visitor(){}
    };

    virtual void accept(Visitor *visitor)const=0;
    virtual ~Stmt(){}

};

struct Var : public Stmt
{

    Var(shared_ptr<const Token> name, shared_ptr<const Expr> initializer):
        name(name), initializer(initializer){}
    void accept(Visitor *visitor)const override
    {
        visitor->visitVarStmt(this);
    }
    shared_ptr<const Token> name;
    shared_ptr<const Expr> initializer;

};


struct Expression : public Stmt
{
    Expression(shared_ptr<const Expr> expr) : expr(expr){}


    void accept(Visitor *visitor) const override
    {
        visitor->visitExpressionStmt(this);
    }

    shared_ptr<const Expr> expr;
};

struct Print : public Stmt
{
    Print(shared_ptr<const Expr> expr) : expr(expr){}

    void accept(Visitor *visitor) const override
    {
        visitor->visitPrintStmt(this);
    }

    shared_ptr<const Expr> expr;
};

struct Block : public Stmt
{
    Block(const vector<shared_ptr<Stmt>> &statements) :
        statements(statements){}
    void accept(Visitor *visitor) const override
    {
        visitor->visitBlockStmt(this);
    }

    vector<shared_ptr<Stmt>> statements;
};

struct If : public Stmt
{
    If(shared_ptr<const Expr> condition, shared_ptr<const Stmt> thenBranch, shared_ptr<const Stmt> elseBranch):
        condition(condition), thenBranch(thenBranch), elseBranch(elseBranch){}
    void accept(Visitor *visitor) const override
    {
        visitor->visitIfStmt(this);
    }
    shared_ptr<const Expr>condition;
    shared_ptr<const Stmt> thenBranch;
    shared_ptr<const Stmt> elseBranch;
};

struct While : public Stmt
{
    While(shared_ptr<const Expr> condition, shared_ptr<const Stmt> body):
        condition(condition), body(body){}
    void accept(Visitor *visitor) const override
    {
        visitor->visitWhileStmt(this);
    }

    shared_ptr<const Expr>condition;
    shared_ptr<const Stmt>body;
};

#endif
