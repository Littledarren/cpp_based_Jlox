/*================================================================
*    
*   
*   FileName: Value.h
*   Author: DarrenHuang
*   Create Time: 2020/07/06  08:30(星期一)
*   Description:
*
================================================================*/
#ifndef _Value_H_
#define _Value_H_

#include "TokenType.h"

//very similar to Token 
//but only holds literal value and its
//runtime type.
//not fore inherited
struct Value
{

    //will new new literal
    Value(TokenType type=NUL, void *literal=nullptr);
    //deep copy
    Value(const Value& v);
    Value& operator=(const Value &v);

    ~Value()
    {
        freeLiteral();
    }

    //for MINUS unary
    Value operator-() const;
    //for BANG unary
    Value operator!() const;
    
    //Arithmetic Binary Operation
    Value operator+(const Value &r) const;
    Value operator-(const Value &r) const;

    Value operator*(const Value &r) const;
    Value operator/(const Value &r) const;


    //comparation
    Value operator<(const Value &r) const;
    Value operator==(const Value &r) const;
    Value operator!=(const Value &r) const;
    Value operator<=(const Value &r) const;
    Value operator>(const Value &r) const;
    Value operator>=(const Value &r) const;
    //    short-cut not supported ...
//    Value operator||(const Value &r) const
//    {
//       if (type == TRUE)  return Value(TRUE);
//    }

    //free resource 
    void freeLiteral();
    //tostring
    string static toString(Value &v);
    //保存运行时类型和指针
    TokenType type;
    void *literal = nullptr;
};
#endif
