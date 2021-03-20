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

/*
 * 环境，或者说作用域，保存变量，函数，类
 *
 */
class Environment : public std::enable_shared_from_this<Environment> {
  using KEY_TYPE = const string;
  using VALUE_TYPE = shared_ptr<Object>;

public:
  Environment(shared_ptr<Environment> enclosing = nullptr)
      : enclosing(enclosing) {}

  void define(KEY_TYPE &key, VALUE_TYPE value);

  VALUE_TYPE get(shared_ptr<Token> name);
  VALUE_TYPE getAt(int dis, KEY_TYPE &name);
  void assign(shared_ptr<Token> name, VALUE_TYPE value);
  void assignAt(int dis, shared_ptr<Token> name, VALUE_TYPE value);

  void print() const;

public:
  shared_ptr<Environment> enclosing;

private:
  shared_ptr<Environment> ancestor(int dis) {
    auto environment = shared_from_this();
    while (dis--) {
      environment = environment->enclosing;
    }
    return environment;
  }

private:
  std::map<KEY_TYPE, VALUE_TYPE> values;
};

#endif
