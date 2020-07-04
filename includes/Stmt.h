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

#include "Expr.h"
struct Expression;
struct Print;

struct Stmt {
    struct Visitor
    {
        virtual void* visitExpressionStmt(Expression *stmt)=0;
        virtual void* visitPrintStmt(Print *stmt)=0;
    };

    virtual void* accept(Visitor *visitor)=0;
    virtual ~Stmt(){}

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



#endif
