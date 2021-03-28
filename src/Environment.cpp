#include "Environment.h"

#include <iostream>

#include "ErrorReporting.h"
#include "Token.h"

namespace clox {
namespace runtime {
using error::RuntimeError;
using std::cout;
using std::endl;

void Environment::define(KEY_TYPE &key, VALUE_TYPE value) noexcept {
  if (values.count(key) != 0) {
    values.at(key) = value;
  } else {
    values.insert(std::make_pair(key, value));
  }
}
Environment::VALUE_TYPE
Environment::get(shared_ptr<Token> name) noexcept(false) {
  if (values.count(name->lexeme) != 0) {
    return values.at(name->lexeme);
  }
  if (enclosing)
    return enclosing->get(name);

  throw RuntimeError(name, string("Undefined variable:") + name->lexeme);
}
Environment::VALUE_TYPE Environment::getAt(int dis, KEY_TYPE &name) noexcept {
  //静态查找是固定的，而运行时查找是不确定的，
  //至于性能。。其实差不了多少，如果是hash map的话。都是一个数量级应该
  return ancestor(dis)->values.at(name);
}
void Environment::assign(shared_ptr<Token> name,
                         VALUE_TYPE value) noexcept(false) {
  if (values.count(name->lexeme) != 0) {
    values.at(name->lexeme) = value;
    return;
  }
  if (enclosing) {
    enclosing->assign(name, value);
    return;
  }
  throw RuntimeError(name, "Undefined variable " + name->lexeme);
}
void Environment::assignAt(int dis, shared_ptr<Token> name,
                           VALUE_TYPE value) noexcept {
  //静态检查已经保证一定存在了
  ancestor(dis)->values.at(name->lexeme) = value;
}

int Environment::dynamic_resolve(KEY_TYPE &name) noexcept {
  int layer = 0;
  if (values.count(name) != 0) {
    return layer;
  }
  if (enclosing)
    return enclosing->dynamic_resolve(name) + 1;
  return -1;
}
void Environment::print() const noexcept {
  for (auto &kv : values) {
    cout << kv.first << "\t:\t" << (kv.second ? (kv.second)->toString() : "Nil")
         << endl;
  }
}
} // namespace runtime
} // namespace clox
