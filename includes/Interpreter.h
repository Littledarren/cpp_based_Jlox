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
#include "Expr.h"

//very similar to Token 
//but only holds literal value and its
//runtime type.
//not fore inherited
struct VALUE_T
{

    VALUE_T(TokenType type, void *literal):
        type(type)
    {
        switch(this->type) {
            case STRING:
                this->literal = new string(*(string*)literal);
                break;
            case NUMBER:
                this->literal = new double(*(double*)literal);
                break;
            case TRUE:
                this->literal = new bool(*(bool*)literal);
                break;
            case FALSE:
                this->literal = new bool(*(bool*)literal);
                break;
            default:
                //default to nullptr
                break;
        }           
    }

    VALUE_T(const VALUE_T& v)
    {
        this->type = v.type;
        //deep copy
        switch(this->type) {
            case STRING:
                this->literal = new string(*(string*)v.literal);
                break;
            case NUMBER:
                this->literal = new double(*(double*)v.literal);
                break;
            case TRUE:
                this->literal = new bool(*(bool*)v.literal);
                break;
            case FALSE:
                this->literal = new bool(*(bool*)v.literal);
                break;
            default:
                //default to nullptr
                break;
        }           
    }

    ~VALUE_T()
    {
        freeLiteral();
    }
    VALUE_T operator+(const VALUE_T &r)
    {
        //will return a new.
        


    }
    VALUE_T operator-(const VALUE_T &r)
    {

    }
    //for MINUS
    VALUE_T& operator-()
    {
       if (type != NUMBER)  throw std::runtime_error("can not do unary - on some NAN");
       double *p = (double*)literal;
       *p = -*p;
       return *this;
    }
    void freeLiteral()
    {
        if (literal == nullptr) return;
        switch(type) {
            case STRING:
                delete (string*)literal;
                break;
            case NUMBER:
                delete (double*)literal;
                break;
            case TRUE:
                delete (bool*)literal;
                break;
            case FALSE:
                delete (bool*)literal;
                break;
            default:
                break;
        }

    }
    //for BANG
    VALUE_T& operator!()
    {
        if (type == FALSE || (type != TRUE && literal == nullptr)) {
            freeLiteral();
            type = TRUE;
            literal = nullptr;
        } else {
            freeLiteral();
            type = FALSE;
            literal =nullptr;
        }
        return *this;
    }
    TokenType type;
    void *literal = nullptr;
};

class Interpreter : public Visitor
{
    
public:
    VALUE_T evaluate(Expr *expr)
    {
        VALUE_T *temp = (VALUE_T*)expr->accept(this);
        //because i define deep copy 
        //so this is allowed;
        VALUE_T result(*temp);
        delete temp;
        //because defined deep copy so allowed
        return result;
    }
    virtual void* visitTernaryExpr(Ternary *expr) override
    {

    }
    virtual void* visitBinaryExpr(Binary *expr) override
    {
        //this will creat a new ..
        VALUE_T *left = (VALUE_T*)expr->left->accept(this); 
        VALUE_T *right = (VALUE_T*)expr->right->accept(this);
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

    }
    virtual void* visitUnaryExpr(Unary *expr) override
    {
        //do not a new VALUE_T to save mem?
        VALUE_T *temp = new VALUE_T(evaluate(expr->right));
        switch(expr->op->type) {
            case MINUS:
                return &-*temp;
            case BANG:
                return &!*temp;
            default:
                throw std::runtime_error("r u kidding me with a wrong unary op?");
        }
    }
    virtual void* visitGroupingExpr(Grouping *expr) override
    {
        return new VALUE_T(evaluate(expr));
    }
    virtual void* visitLiteralExpr(Literal *expr) override
    {
        return new VALUE_T(*(expr->type), expr->value);
    }
};

#endif
