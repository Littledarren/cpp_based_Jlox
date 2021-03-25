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

#include <sstream>

#include "Object.h"

namespace clox {
using std::ostringstream;
namespace value {

struct Number;
struct Bool;
struct String;

shared_ptr<String> StringAdd(const shared_ptr<String> &str,
                             const shared_ptr<Object> &ob) noexcept;
// 没啥用，还麻烦，不好弄。。。
// struct Nil;
//
//
// struct Nil : public virtual Object
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
// private:
//    Nil()=default;
//    Nil(const Nil& o)=delete;
//    Nil(Nil&& o)=delete;
//};

struct Number : public virtual Object {
  Number(double value = 0.0) : value(value) {}
  operator double() { return value; }
  operator double() const { return value; }

  bool operator==(const Object &r) const noexcept override;
  string toString() const noexcept override;

  virtual shared_ptr<Object> clone() const noexcept override {
    return std::make_shared<Number>(value);
  }

  virtual bool isTrue() const noexcept override { return value != 0; }

public:
  double value;
};

struct Bool : public virtual Object {
  Bool(bool value = false) : value(value) {}
  operator bool() { return value; }
  operator bool() const { return value; }

  bool operator==(const Object &r) const noexcept override;

  string toString() const noexcept override { return value ? "True" : "False"; }
  virtual shared_ptr<Object> clone() const noexcept override {
    return std::make_shared<Bool>(value);
  }
  virtual bool isTrue() const noexcept override { return value; }

public:
  bool value;
};

struct String : public virtual Object {

  friend shared_ptr<String> StringAdd(const shared_ptr<String> &str,
                                      const shared_ptr<Object> &ob) noexcept;
  String(const char *value = "") : value(value) {}
  String(const string &value = "") : value(value) {}
  operator string() { return value; }
  operator string() const { return value; }

  bool operator==(const Object &r) const noexcept override;

  string toString() const noexcept override { return value; }
  virtual shared_ptr<Object> clone() const noexcept override {
    return std::make_shared<String>(value);
  }

  virtual bool isTrue() const noexcept override { return !value.empty(); }
  String operator+(const Object &obj) const noexcept;

public:
  string value;
};

} // namespace value
} // namespace clox

#endif
