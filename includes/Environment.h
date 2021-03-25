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
namespace clox {

namespace token {
struct Token;
}

namespace runtime {
using std::shared_ptr;
using std::string;
using namespace value;
using namespace token;

/*
 * 环境，或者说作用域，保存变量，函数，类
 *
 */
class Environment : public std::enable_shared_from_this<Environment> {
  using KEY_TYPE = const string;
  using VALUE_TYPE = shared_ptr<Object>;

public:
  //只容许使用make_shared构造
  Environment(shared_ptr<Environment> enclosing = nullptr) noexcept
      : enclosing(enclosing) {}

  void define(KEY_TYPE &key, VALUE_TYPE value) noexcept;

  VALUE_TYPE get(shared_ptr<Token> name) noexcept(false);
  VALUE_TYPE getAt(int dis, KEY_TYPE &name) noexcept;
  void assign(shared_ptr<Token> name, VALUE_TYPE value) noexcept(false);
  void assignAt(int dis, shared_ptr<Token> name, VALUE_TYPE value) noexcept;

  void print() const noexcept;

public:
  shared_ptr<Environment> enclosing;

private:
  shared_ptr<Environment> ancestor(int dis) noexcept {
    auto environment = shared_from_this();
    while (dis--) {
      environment = environment->enclosing;
    }
    return environment;
  }

private:
  std::map<KEY_TYPE, VALUE_TYPE> values;
};

} // namespace runtime
} // namespace clox

#endif
