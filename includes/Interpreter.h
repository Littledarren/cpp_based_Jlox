/*================================================================
*    
*   
*   FileName: Interpreter.h
*   Author: DarrenHuang
*   Create Time: 2020/06/20  16:37(Saturday)
*   Description:
*
================================================================*/
#ifndef _INTERPRETER_H_
#define _INTERPRETER_H_

#include <initializer_list>
#include <vector>
using std::vector;

#include "Expr.h"
#include "Stmt.h"
#include "TokenType.h"
#include "Value.h"

#include "Environment.h"


class Interpreter : public Expr::Visitor, public Stmt::Visitor
{
    
public:
    Interpreter():environment(new Environment()){}
    ~Interpreter() {
        delete environment;
    }
    void interprete(vector<Stmt*> statements);

    Value interprete(Expr *expr);
    void* evaluate(Expr *expr);
    void execute(Stmt * stmt);
    void executeBlock(vector<Stmt*> stmts, Environment *environment);
    void* visitAssignExpr(Assign *expr) override;
    virtual void* visitTernaryExpr(Ternary *expr) override;
    virtual void* visitBinaryExpr(Binary *expr) override;
    virtual void* visitUnaryExpr(Unary *expr) override;
    virtual void* visitGroupingExpr(Grouping *expr) override;
    virtual void* visitLiteralExpr(Literal *expr) override;
    virtual void* visitVariableExpr(Variable *expr) override;
    virtual void * visitLogicalExpr(Logical *expr) override;

    void* visitExpressionStmt(Expression *stmt) override;
    void* visitPrintStmt(Print *stmt) override;
    void* visitVarStmt(Var *stmt) override;
    void* visitBlockStmt(Block *stmt) override;
    void* visitIfStmt(If *stmt) override;
    //aditional funcs for debug
    void printEnvironment();
private:
    //check operant type
    void static checkStringOrNumber(TokenType op, const Value &l, const Value &r);
    void static chechNumber(TokenType op, const Value &v);
    void static chechNumber(TokenType op, const Value &l, const  Value &r);
    Environment *environment;

};

#endif
