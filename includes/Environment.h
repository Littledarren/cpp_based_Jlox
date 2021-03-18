/*================================================================
*
*
*   FileName: Environment.h
*   Author: DarrenHuang
*   Create Time: 2020/07/05  18:03(星期日)
*   Description:
*
================================================================*/
#ifndef _ENVIRONMENT_H_
#define _ENVIRONMENT_H_

#include <map>
#include <memory>
#include <string>

#include "Value.h"
struct Token;

using std::shared_ptr;
using std::string;

class Environment {
public:
  Environment(shared_ptr<Environment> enclosing = nullptr)
      : enclosing(enclosing) {}

  void define(const string &name, shared_ptr<Object> value);
  shared_ptr<Object> get(shared_ptr<Token> name);
  shared_ptr<Object> getAt(int dis, const string &name);
  void assign(shared_ptr<Token> name, shared_ptr<Object> value);
  void assignAt(int dis, shared_ptr<Token> name, shared_ptr<Object> value);

  void print() const;

public:
  shared_ptr<Environment> enclosing;

private:
  Environment *ancestor(int dis) {
    Environment *environment = this;
    while (dis--) {
      environment = environment->enclosing.get();
      //                         ^^^^^^^^^->shared_ptr;
    }
    return environment;
  }

private:
  std::map<const string, shared_ptr<Object>> values;
};

#endif
