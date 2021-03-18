#include "../includes/Environment.h"

#include <iostream>

#include "../includes/RuntimeError.h"
#include "../includes/Token.h"
using std::cout;
using std::endl;

void Environment::define(const string &name, shared_ptr<Object> value) {
  if (values.count(name) != 0) {
    values.at(name) = value;
  } else {
    values.insert(std::pair<const string, shared_ptr<Object>>(name, value));
  }
}
shared_ptr<Object> Environment::get(shared_ptr<Token> name) {
  if (values.count(name->lexeme) != 0) {
    return values.at(name->lexeme);
  }
  if (enclosing)
    return enclosing->get(name);
  throw RuntimeError(name, string("Undefined variable:") + name->lexeme);
}
shared_ptr<Object> Environment::getAt(int dis, const string &name) {
  //静态查找是固定的，而运行时查找是不确定的，
  //至于性能。。其实差不了多少，如果是hash map的话。都是一个数量级应该
  return ancestor(dis)->values.at(name);
}
void Environment::assign(shared_ptr<Token> name, shared_ptr<Object> value) {
  if (values.count(name->lexeme) != 0) {
    values.at(name->lexeme) = value;
    return;
  }
  if (enclosing) {
    enclosing->assign(name, value);
    return;
  }
  throw RuntimeError(name, string("Undefined variable") + name->lexeme);
}
void Environment::assignAt(int dis, shared_ptr<Token> name,
                           shared_ptr<Object> value) {
  //静态检查已经保证一定存在了
  ancestor(dis)->values.at(name->lexeme) = value;
}
// const std::map<string, Object*>& Environment::getObjects() const
//{
//    return this->values;
//}
void Environment::print() const {
  for (auto kv : values) {
    cout << kv.first << "\t:\t" << (kv.second)->toString() << endl;
  }
}
