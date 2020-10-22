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
#include <memory>
#include <vector>

#include "Expr.h"
#include "Stmt.h"
#include "TokenType.h"
#include "Value.h"

#include "Environment.h"
using std::vector;
using std::unique_ptr;

typedef shared_ptr<Stmt> StmtPtr;
typedef shared_ptr<const Stmt> CStmtPtr;
typedef shared_ptr<const Expr> CExprPtr;
typedef shared_ptr<const Object> CObjectPtr;

// tree walker
class Interpreter : public Expr::Visitor, public Stmt::Visitor
{

public:
    Interpreter():environment(new Environment()){}
    void interprete(vector<StmtPtr> statements);

    CObjectPtr interprete(shared_ptr<const Expr>expr);
    CObjectPtr evaluate(shared_ptr<const Expr>expr);
    void execute(shared_ptr<const Stmt>  stmt);
    void executeBlock(vector<StmtPtr> stmts, shared_ptr<Environment> environment);
    //expr
    CObjectPtr visitAssignExpr(const Assign *expr) override;
    CObjectPtr visitBinaryExpr(const Binary *expr) override;
    CObjectPtr visitGroupingExpr(const Grouping *expr) override;
    CObjectPtr visitLiteralExpr(const Literal *expr) override;
    CObjectPtr visitUnaryExpr(const Unary *expr) override;
    CObjectPtr visitTernaryExpr(const Ternary *expr) override;
    CObjectPtr visitLogicalExpr(const Logical *expr) override;
    CObjectPtr visitVariableExpr(const Variable *expr) override;
    CObjectPtr visitCallExpr(const Call *expr) override;
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
    static void checkStringOrNumber(shared_ptr<const Token>op, CObjectPtr l, CObjectPtr r);
    static void chechNumber(shared_ptr<const Token>op, CObjectPtr v);
    static void chechNumber(shared_ptr<const Token>op, CObjectPtr l, CObjectPtr r);
    shared_ptr<Environment> environment;

};

#endif
