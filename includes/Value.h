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

#include <vector>
using std::vector;

#include "Object.h"

#include <sstream>
using std::ostringstream;

struct Number;
struct Bool;
struct String;

struct Number : public virtual Object
{
    Number(double value=0.0):value(value){}
    operator double()
    {
        return value;
    }
    operator double() const
    {
        return value;
    }
    bool operator==(const Object &r) const override
    {
        const Number* rp = dynamic_cast<const Number*>(&r);
        if (rp == nullptr) return false;
        return value == rp->value;
    }
    string toString() const override
    {
        std::ostringstream oss;
        oss<<value;
        return oss.str();
    }
    double value;
};

struct Bool : public virtual Object
{
    Bool(bool value): value(value){}

    operator bool()
    {
        return value;
    }
    operator bool() const
    {
        return value;
    }
    bool operator==(const Object &r) const override
    {
        const Bool* rp = dynamic_cast<const Bool*>(&r);
        if (rp == nullptr) return false;
        return value == rp->value;
    }

    string toString() const override
    {
        return value?"True":"False";
    }

    bool value;
};


struct String : public virtual Object
{
    String(const char *value):value(value){}
    String(const string &value):value(value){}
    operator string()
    {
        return value;
    }
    operator string() const
    {
        return value;
    }
    bool operator==(const Object &r) const override
    {
        const String* rp = dynamic_cast<const String*>(&r);
        if (rp == nullptr) return false;
        return value == rp->value;
    }

    string toString() const override
    {
        return value;
    }
    String operator+(const Number &value) const;
    String operator+(const Bool &value) const;
    string value;
};


class Interpreter;

struct Callable : public virtual Object
{
    virtual const Object* call(Interpreter* interpreter, const vector<const Object*> &arguments)const=0;
    int arity();
};



#endif
