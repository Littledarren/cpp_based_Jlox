/*================================================================
*    
*   
*   FileName: Value.h
*   Author: DarrenHuang
*   Create Time: 2020/07/06  08:30(星期一)
*   Description:
*           Booleans, Numbers, Strings, NIl
*
================================================================*/
#ifndef _Value_H_
#define _Value_H_

#include <vector>
#include <sstream>
#include <functional>
#include <stdexcept>
using std::runtime_error;
using std::ostringstream;
using std::vector;


#include "Object.h"

struct Number;
struct Bool;
struct String;

// 没啥用，还麻烦，不好弄。。。
//struct Nil;
//
//
//struct Nil : public virtual Object
//{
//    virtual shared_ptr<Object> clone() const  override
//    {
//        return getInstance();
//    }
//    string toString() const override
//    {
//        std::ostringstream oss;
//        oss<<"Nil";
//        return oss.str();
//    }
//    bool operator==(const Object &r) const override
//    {
//        return false;
//    }
//    static Nil& getInstance() 
//    {
//        static Nil instance;
//        return instance;
//    }
//private:
//    Nil()=default;
//    Nil(const Nil& o)=delete;
//    Nil(Nil&& o)=delete;
//};


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
        if (rp == nullptr) 
            throw runtime_error("Number class should not be compared with nullptr");
        return value == rp->value;
    }
    string toString() const override
    {
        std::ostringstream oss;
        oss<<value;
        return oss.str();
    }
    virtual shared_ptr<Object> clone() const  override
    {
        return std::make_shared<Number>(value);
    }
    virtual bool isTrue() const override{
        return value != 0;
    }
    double value;
};

struct Bool : public virtual Object
{
    Bool(bool value=false): value(value){}

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
    virtual shared_ptr<Object> clone() const override
    {
        return std::make_shared<Bool>(value);
    }
    virtual bool isTrue() const override{
        return value;
    }

    bool value;
};


struct String : public virtual Object
{
    String(const char *value=""):value(value){}
    String(const string &value=""):value(value){}
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
    virtual shared_ptr<Object> clone() const override
    {
        return std::make_shared<String>(value);
    }
    virtual bool isTrue() const override{
        return !value.empty();
    }
    String operator+(const shared_ptr<Object> &value) const;
    String operator+(const Number &value) const;
    String operator+(const Bool &value) const;
    string value;
};

#endif
