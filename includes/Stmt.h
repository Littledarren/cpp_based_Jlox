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

struct Stmt {
    struct Visitor
    {
        virtual void* visitExpressionStmt(Expression *stmt)=0;
        virtual void* visitPrintStmt(Print *stmt)=0;
        virtual void* visitVarStmt(Var *stmt)=0;
        virtual void* visitBlockStmt(Block *stmt)=0;
        virtual void* visitIfStmt(If *stmt)=0;
        virtual ~Visitor(){}
    };

    virtual void* accept(Visitor *visitor)=0;
    virtual ~Stmt(){}

};

struct Var : public Stmt
{

    Var(Token * const &name, Expr * const &initializer):
        name(name), initializer(initializer){}
    ~Var()
    {
        //name will be freed by Lexer.
        delete initializer;
    }
    void * accept(Visitor *visitor) override
    {
        return visitor->visitVarStmt(this);
    }
    Token * name;
    Expr * initializer;

};


struct Expression : public Stmt
{
    Expression(Expr *expr) : expr(expr){}
    virtual ~Expression()
    {
        delete expr;
    };


    void * accept(Visitor *visitor) override
    {
        return visitor->visitExpressionStmt(this);
    }

    Expr* const expr;
};

struct Print : public Stmt
{
    Print(Expr *expr) : expr(expr){}
    virtual ~Print()
    {
        delete expr;
    };

    void * accept(Visitor *visitor) override
    {
        return visitor->visitPrintStmt(this);
    }

    Expr* const expr;
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
    void * accept(Visitor *visitor) override
    {
        return visitor->visitBlockStmt(this);
    }

    vector<Stmt*> statements;
};

struct If : public Stmt
{
   If(Expr *condition, Stmt *thenBranch, Stmt *elseBranch):
       condition(condition), thenBranch(thenBranch), elseBranch(elseBranch){}
   void * accept(Visitor *visitor) override
   {
       return visitor->visitIfStmt(this);
   }
   virtual ~If()
   {
        delete condition;
        delete thenBranch;
        delete elseBranch;
   }
   Expr *condition;
   Stmt *thenBranch;
   Stmt *elseBranch;
};


#endif
