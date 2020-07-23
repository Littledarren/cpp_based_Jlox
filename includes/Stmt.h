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

    Var(const Token* name, const Expr* initializer):
        name(name), initializer(initializer){}
    ~Var()
    {
        //name will be freed by Lexer.
        delete initializer;
    }
    void accept(Visitor *visitor)const override
    {
        visitor->visitVarStmt(this);
    }
    const Token* name;
    const Expr* initializer;

};


struct Expression : public Stmt
{
    Expression(const Expr* expr) : expr(expr){}
    virtual ~Expression()
    {
        delete expr;
    };


    void accept(Visitor *visitor) const override
    {
        visitor->visitExpressionStmt(this);
    }

    const Expr* expr;
};

struct Print : public Stmt
{
    Print(const Expr* expr) : expr(expr){}
    virtual ~Print()
    {
        delete expr;
    };

    void accept(Visitor *visitor) const override
    {
        visitor->visitPrintStmt(this);
    }

    const Expr* expr;
};

struct Block : public Stmt
{
    Block(const vector<Stmt*> &statements) :
        statements(statements){}
    virtual ~Block()
    {
        for (auto p : statements)
            delete p;
    }
    void accept(Visitor *visitor) const override
    {
        visitor->visitBlockStmt(this);
    }

    vector<Stmt*> statements;
};

struct If : public Stmt
{
   If(const Expr* condition, const Stmt* thenBranch, const Stmt* elseBranch):
       condition(condition), thenBranch(thenBranch), elseBranch(elseBranch){}
   void accept(Visitor *visitor) const override
   {
       visitor->visitIfStmt(this);
   }
   virtual ~If()
   {
        delete condition;
        delete thenBranch;
        delete elseBranch;
   }
   const Expr *condition;
   const Stmt *thenBranch;
   const Stmt *elseBranch;
};

struct While : public Stmt
{
    While(const Expr* condition, const Stmt* body):
        condition(condition), body(body){}
    virtual ~While()
    {
        delete condition;
        delete body;
    }
    void accept(Visitor *visitor) const override
    {
       visitor->visitWhileStmt(this);
    }

    const Expr *condition;
    const Stmt *body;
};

#endif
