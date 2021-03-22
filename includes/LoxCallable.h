/*================================================================



*
*
*   FileName: LoxCallable.h
*   Author: DarrenHuang
*   Create Time: 2021/03/16  17:18(星期二)
*   Description:
*
================================================================*/
#ifndef _LOXCALLABLE_H_
#define _LOXCALLABLE_H_

#include <stdexcept>

#include "LoxInstance.h"
#include "Object.h"
#include "Stmt.h"
#include "Value.h"

class Interpreter;
class Environment;

struct Control : public runtime_error {
  Control(const shared_ptr<Object> &data, TokenType type)
      : runtime_error(getNameOfType(type)), type(type), data(data) {}

  TokenType type;
  shared_ptr<Object> data;
};

struct Callable : public virtual Object {
  virtual shared_ptr<Object> call(Interpreter &,
                                  const vector<shared_ptr<Object>> &) = 0;
  virtual int arity() = 0;
  virtual ~Callable() = default;
};

struct NativeFunction : public Callable {
  virtual string toString() const override { return "<native fn>"; }
  virtual ~NativeFunction() = default;
};

struct Clock : public NativeFunction {
  virtual int arity() override { return 0; }
  virtual shared_ptr<Object> call(Interpreter &,
                                  const vector<shared_ptr<Object>> &) override {
    return std::make_shared<Number>((double)time(0));
  }
};

struct LoxInstance;
struct LoxFunction : public Callable {
  LoxFunction(shared_ptr<Function> func, shared_ptr<Environment> closure,
              bool isInitializer)
      : declaration(func), closure(closure), isInitializer(isInitializer) {}

  virtual shared_ptr<Object>
  call(Interpreter &interpreter,
       const vector<shared_ptr<Object>> &arguments) override;

  virtual int arity() override { return declaration->lambda->params.size(); }

  virtual string toString() const override {
    if (declaration->name)
      return "<userdefined fn:" + declaration->name->lexeme + ">";
    else
      return "<userdefined lambda fn>";
  }

  shared_ptr<LoxFunction> bind(shared_ptr<LoxInstance> owner);

  virtual ~LoxFunction() = default;

private:
  shared_ptr<Function> declaration;
  shared_ptr<Environment> closure;
  bool isInitializer;
};

#include <map>
using std::map;
struct LoxClass : public Callable, public LoxInstance {
  using FIELD_TYPE = shared_ptr<Object>;

  LoxClass(const string &name,
           const map<string, shared_ptr<LoxFunction>> &methods)
      : LoxInstance(this), name(name), methods(methods) {}

  virtual ~LoxClass() = default;
  virtual shared_ptr<Object>
  call(Interpreter &interpreter,
       const vector<shared_ptr<Object>> &arguments) override;
  virtual int arity() override;
  virtual string toString() const override { return name; }

  shared_ptr<LoxFunction> findMethod(string methodName) {
    return methods[methodName];
  }

public:
  string name;
  map<string, shared_ptr<LoxFunction>> methods;
};

#endif
