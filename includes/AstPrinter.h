/*================================================================
 *    
 *   
 *   FileName: AstPrinter.h
 *   Author: DarrenHuang
 *   Create Time: 2020/06/18  09:15(Thursday)
 *   Description:
 *
 ================================================================*/
#ifndef _ASTPRINTER_H_
#define _ASTPRINTER_H_

#include <initializer_list>
#include "Expr.h"
//print AST
class AstPrinter : public Visitor
{

public:
    //since c++ do not support automatic dynamic interpret
    //so we have to use void* and do dynamic cast ourselves
    //This already makes me fool.
    string print(Expr *expr)
    {
        string *temp = static_cast<string*>(expr->accept(this));

        string result(*temp);
        delete temp;
        return result;
    }
    virtual void* visitTernaryExpr(Ternary *expr) override
    {
        return parenthesize("ternary ?:", {expr->condition, expr->if_no, expr->if_yes});
    }
    virtual void* visitBinaryExpr(Binary *expr) override
    {
        return parenthesize(expr->op->lexeme, {expr->left, expr->right});
    }
    virtual void* visitGroupingExpr(Grouping *expr) override
    {
        return parenthesize("group", {expr->expr});
    }
    virtual void* visitLiteralExpr(Literal *expr) override
    {

        if (expr->value == nullptr) return new string("nil");
        return expr->getValStr();
    }
    virtual void* visitUnaryExpr(Unary *expr) override
    {
        return parenthesize(expr->op->lexeme, {expr->right});
    }
private:
    void* parenthesize(const string &name, std::initializer_list<Expr *> exprs)
    {

        string *result = new string(string("(") + name);
        for (auto p : exprs) {
            string *temp = static_cast<string*>(p->accept(this));
            *result += " " + *temp;
            delete temp;
        }
        
        *result += ")";
        return result;
    }


};

//show RPN 
class Evaluator : public Visitor
{
public:
    string RPN(Expr *expr)
    {
        string *temp = static_cast<string*>(expr->accept(this));

        string result(*temp);
        delete temp;
        return result;
    }
    virtual void* visitTernaryExpr(Ternary *expr) override
    {
        //i dont know how to represent ternary expr in RPN
        return nullptr;
    }
    virtual void* visitBinaryExpr(Binary *expr) override
    {
        string *left = static_cast<string*>(expr->left->accept(this));
        string *right = static_cast<string*>(expr->right->accept(this));
        string op = expr->op->lexeme;
        string *result = new string(*left + " " + *right + " " + op);
        delete left;
        delete right;
        return result;
    }
    virtual void* visitGroupingExpr(Grouping *expr) override
    {
        return expr->expr->accept(this);
    }
    virtual void* visitLiteralExpr(Literal *expr) override
    {
        if (expr->value == nullptr) return new string("nil");
        return expr->getValStr();
    }
    virtual void* visitUnaryExpr(Unary *expr) override
    {
        string parent = expr->op->lexeme;
        string *child = static_cast<string*>(expr->right->accept(this));
        string *result = new string(*child + " " + parent);
        delete child;
        return result;
    }
};
// caculate double value
class Calculator : public Visitor
{

public:
    double eval(Expr *expr)
    {
        try {
            double *temp = static_cast<double*>(expr->accept(this));
            double result = *temp;
            delete temp;
            return result;
        } catch(const char *e) {
            std::cout<<e<<std::endl;
            return 0;
        }
    }
    virtual void* visitTernaryExpr(Ternary *expr) override
    {
        //sometimes i dont know the type of expression
        //is it a boolean or a number 
        //or string. That depends.
        double *cond = static_cast<double*>(expr->condition->accept(this));
        if (*cond <0.00000000001 && *cond >0.00000000001) {
            double *no = static_cast<double*>(expr->if_no->accept(this));
            delete cond;
            return no;
        } else {
            double *yes = static_cast<double*>(expr->if_yes->accept(this));
            delete cond;
            return yes;
        }

        return nullptr;
    }
    virtual void* visitBinaryExpr(Binary *expr) override
    {
        double *left_p = static_cast<double*>(expr->left->accept(this));
        double *right_p = static_cast<double*>(expr->right->accept(this));
        double &left = *left_p;
        double &right = *right_p;
        double result;
        switch(expr->op->type)
        {
            case PLUS:
                result =  left + right;
                break;
            case MINUS:
                result = left - right;
                break;
            case STAR:
                result =  left * right;
                break;
            case SLASH:
                result = left / right;
                break;
            case COMMA:
                result = right;
                break;
            default:
                throw "ERROR UNKONE OP" + expr->op->lexeme;
        }
        delete left_p;
        delete right_p;
        return new double(result);
    }
    virtual void* visitGroupingExpr(Grouping *expr) override
    {
        return expr->expr->accept(this);
    }
    virtual void* visitLiteralExpr(Literal *expr) override
    {
        if (expr->value == nullptr) 
            throw "ERROR ILLEGAL VALUE NIL";
        if (*(expr->type) != NUMBER)
            throw "VALUE TYPE IS NOT NUMBER";
        return new double(expr->getDouble());
    }
    virtual void* visitUnaryExpr(Unary *expr) override
    {
        double *right_p = static_cast<double*>(expr->right->accept(this));
        double result;
        switch(expr->op->type)
        {
            case MINUS:
                result = -*right_p;
                break;
            default:
                throw "UNKNOWN UNARY OPERATION" + expr->op->lexeme;
        }

        return new double(result);
    }
};
#endif
