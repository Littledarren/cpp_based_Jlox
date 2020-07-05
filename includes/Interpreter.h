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
#include "Environment.h"

class RuntimeError : public std::runtime_error
{
public:
    RuntimeError(TokenType type, const char *str) : runtime_error(str), type(type)
    {}
    
    const TokenType type;
};
//very similar to Token 
//but only holds literal value and its
//runtime type.
//not fore inherited
struct VALUE_T
{

    //will new new literal
    VALUE_T(TokenType type=NUL, void *literal=nullptr);
    //deep copy
    VALUE_T(const VALUE_T& v);
    VALUE_T& operator=(const VALUE_T &v);

    ~VALUE_T()
    {
        freeLiteral();
    }

    //for MINUS unary
    VALUE_T operator-() const;
    //for BANG unary
    VALUE_T operator!() const;
    
    //Arithmetic Binary Operation
    VALUE_T operator+(const VALUE_T &r) const;
    VALUE_T operator-(const VALUE_T &r) const;

    VALUE_T operator*(const VALUE_T &r) const;
    VALUE_T operator/(const VALUE_T &r) const;


    //comparation
    VALUE_T operator<(const VALUE_T &r) const;
    VALUE_T operator==(const VALUE_T &r) const;
    VALUE_T operator!=(const VALUE_T &r) const;
    VALUE_T operator<=(const VALUE_T &r) const;
    VALUE_T operator>(const VALUE_T &r) const;
    VALUE_T operator>=(const VALUE_T &r) const;
    //    short-cut not supported ...
//    VALUE operator||(const VALUE_T &r) const
//    {
//       if (type == TRUE)  return VALUE_T(TRUE);
//    }

    //free resource 
    void freeLiteral();
    //check operant type
    void static checkStringOrNumber(TokenType op, const VALUE_T &l, const VALUE_T &r);
    void static chechNumber(TokenType op, const VALUE_T &v);
    void static chechNumber(TokenType op, const VALUE_T &l, const  VALUE_T &r);
    //tostring
    string static toString(VALUE_T &v);
    //保存运行时类型和指针
    TokenType type;
    void *literal = nullptr;
};

class Interpreter : public Expr::Visitor, public Stmt::Visitor
{
    
public:
    void interprete(vector<Stmt*> statements);

    VALUE_T interprete(Expr *expr);
    void* evaluate(Expr *expr);
    void execute(Stmt * stmt);
    virtual void* visitTernaryExpr(Ternary *expr) override;
    virtual void* visitBinaryExpr(Binary *expr) override;
    virtual void* visitUnaryExpr(Unary *expr) override;
    virtual void* visitGroupingExpr(Grouping *expr) override;
    virtual void* visitLiteralExpr(Literal *expr) override;
    virtual void * visitVariableExpr(Variable *expr) override;

    void * visitExpressionStmt(Expression *stmt) override;
    void * visitPrintStmt(Print *stmt) override;
    void * visitVarStmt(Var *stmt) override;
private:
    Environment environment;

};

#endif
