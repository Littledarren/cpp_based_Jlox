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

    const Object* interprete(const Expr *expr);
    const Object* evaluate(const Expr *expr);
    void execute(const Stmt * stmt);
    void executeBlock(vector<Stmt*> stmts, Environment *environment);
    //expr
    const Object* visitAssignExpr(const Assign *expr) override;
    const Object* visitBinaryExpr(const Binary *expr) override;
    const Object* visitGroupingExpr(const Grouping *expr) override;
    const Object* visitLiteralExpr(const Literal *expr) override;
    const Object* visitUnaryExpr(const Unary *expr) override;
    const Object* visitTernaryExpr(const Ternary *expr) override;
    const Object* visitLogicalExpr(const Logical *expr) override;
    const Object* visitVariableExpr(const Variable *expr) override;
    const Object* visitCallExpr(const Call *expr) override;
    //Stmt
    void visitExpressionStmt(const Expression *stmt) override;
    void visitPrintStmt(const Print *stmt) override;
    void visitVarStmt(const Var *stmt) override;
    void visitBlockStmt(const Block *stmt) override;
    void visitIfStmt(const If *stmt) override;
    void visitWhileStmt(const While *stmt) override;

    //aditional funcs for debug
    void printEnvironment();
private:
    //check operant type
    static void checkStringOrNumber(const Token *op, const Object &l, const Object &r);
    static void chechNumber(const Token *op, const Object &v);
    static void chechNumber(const Token *op, const Object &l, const Object &r);
    Environment *environment;

};

#endif
