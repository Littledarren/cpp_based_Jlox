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

#include "ErrorReporting.h"
#include "LoxInstance.h"
#include "Stmt.h"
#include "Value.h"

namespace clox {
namespace runtime {

class Interpreter;
class Environment;
using namespace compiling;
using token::TokenType;
using value::LoxInstance;
using value::Object;

//用于控制流，如break, continue, return
struct Control : public runtime_error {
  Control(TokenType type, const shared_ptr<token::Object> &data) noexcept
      : runtime_error(getNameOfType(type)), type(type), data(data) {}

  TokenType type;
  shared_ptr<Object> data;
};

////////////////////////////////////////////////////////////////////////
//                                 借口                               //
////////////////////////////////////////////////////////////////////////

struct Callable : public virtual Object {
  virtual shared_ptr<Object> call(Interpreter &,
                                  const vector<shared_ptr<Object>> &) = 0;
  virtual int arity() noexcept = 0;
  virtual ~Callable() noexcept = default;
};

////////////////////////////////////////////////////////////////////////
//                          Native Functions                          //
////////////////////////////////////////////////////////////////////////

struct NativeFunction : public Callable {
  virtual string toString() const noexcept { return "<native fn>"; }
  virtual ~NativeFunction() noexcept = default;
};

struct Clock : public NativeFunction {
  virtual int arity() noexcept override { return 0; }
  virtual shared_ptr<Object> call(Interpreter &,
                                  const vector<shared_ptr<Object>> &) override {
    return std::make_shared<value::Number>((double)time(0));
  }
};

struct Input : public NativeFunction {
  virtual int arity() noexcept override { return 0; }
  virtual shared_ptr<Object> call(Interpreter &,
                                  const vector<shared_ptr<Object>> &) override {
    string temp;
    std::getline(std::cin, temp);
    return std::make_shared<value::String>(temp);
  }
};
////////////////////////////////////////////////////////////////////////
//                              Function                              //
////////////////////////////////////////////////////////////////////////

struct LoxFunction : public Callable {
  LoxFunction(shared_ptr<Function> func, shared_ptr<Environment> closure)
      : declaration(func), closure(closure), hold_closure(nullptr) {}

  virtual shared_ptr<Object>
  call(Interpreter &interpreter,
       const vector<shared_ptr<Object>> &arguments) override;

  virtual int arity() noexcept override {
    return declaration->lambda->params.size();
  }

  virtual string toString() const noexcept override {
    if (declaration->name)
      return "<user defined fn:" + declaration->name->lexeme + ">";
    else
      return "<user defined lambda fn>";
  }

  shared_ptr<LoxFunction> bind(shared_ptr<LoxInstance> owner) noexcept;

  shared_ptr<LoxFunction> hold() noexcept {
    auto closure_func =
        std::make_shared<LoxFunction>(declaration, closure.lock());
    closure_func->hold_closure = closure.lock();
    return closure_func;
  }

  virtual ~LoxFunction() = default;

public:
  shared_ptr<Function> declaration;

private:
  // !!!!! 任何值类型不能保留Environment的强引用
  // std::weak_ptr<Environment> closure;
  // 2021-03-26
  // 没办法。。。必须要有强引用，闭包需要
  std::weak_ptr<Environment> closure;
  shared_ptr<Environment> hold_closure;
};

using std::map;
struct LoxClass : public Callable, public LoxInstance {
  using FIELD_TYPE = shared_ptr<Object>;

  LoxClass(const string &name, shared_ptr<LoxClass> super_class,
           const map<string, shared_ptr<LoxFunction>> &methods) noexcept
      : LoxInstance(), name(name), super_class(super_class), methods(methods) {}

  virtual ~LoxClass() noexcept = default;
  virtual shared_ptr<Object>
  call(Interpreter &interpreter,
       const vector<shared_ptr<Object>> &arguments) override;
  virtual int arity() noexcept override;
  virtual string toString() const noexcept override { return name; }

  shared_ptr<LoxFunction> findMethod(string methodName) noexcept {
    if (methods.find(methodName) != methods.end())
      return methods[methodName];
    else if (super_class)
      return super_class->findMethod(methodName);
    return nullptr;
  }

public:
  string name;
  shared_ptr<LoxClass> super_class;
  map<string, shared_ptr<LoxFunction>> methods;
};
} // namespace runtime
} // namespace clox

#endif
