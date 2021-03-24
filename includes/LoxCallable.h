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
  Control(TokenType type, const shared_ptr<token::Object> &data)
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
  virtual int arity() = 0;
  virtual ~Callable() = default;
};

////////////////////////////////////////////////////////////////////////
//                          Native Functions                          //
////////////////////////////////////////////////////////////////////////

struct NativeFunction : public Callable {
  virtual string toString() const { return "<native fn>"; }
  virtual ~NativeFunction() = default;
};

struct Clock : public NativeFunction {
  virtual int arity() override { return 0; }
  virtual shared_ptr<Object> call(Interpreter &,
                                  const vector<shared_ptr<Object>> &) override {
    return std::make_shared<value::Number>((double)time(0));
  }
};
////////////////////////////////////////////////////////////////////////
//                              Function                              //
////////////////////////////////////////////////////////////////////////

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
      return "<user defined fn:" + declaration->name->lexeme + ">";
    else
      return "<user defined lambda fn>";
  }

  shared_ptr<LoxFunction> bind(shared_ptr<LoxInstance> owner);

  virtual ~LoxFunction() = default;

public:
  shared_ptr<Function> declaration;

private:
  shared_ptr<Environment> closure;
  //应该没有必要了。必要的信息应该保存在编译时中
  bool isInitializer;
};

using std::map;
struct LoxClass : public Callable, public LoxInstance {
  using FIELD_TYPE = shared_ptr<Object>;

  LoxClass(const string &name, shared_ptr<LoxClass> super_class,
           const map<string, shared_ptr<LoxFunction>> &methods)
      : LoxInstance(this), name(name), super_class(super_class),
        methods(methods) {}

  virtual ~LoxClass() = default;
  virtual shared_ptr<Object>
  call(Interpreter &interpreter,
       const vector<shared_ptr<Object>> &arguments) override;
  virtual int arity() override;
  virtual string toString() const override { return name; }

  shared_ptr<LoxFunction> findMethod(string methodName) {
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
