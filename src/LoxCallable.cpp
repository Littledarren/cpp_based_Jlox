/*================================================================
*
*
*   FileName: LoxCallable.cpp
*   Author: DarrenHuang
*   Create Time: 2021/03/20  13:33(星期六)
*   Description:
*
================================================================*/
#include "LoxCallable.h"

#include "Interpreter.h"
#include "LoxInstance.h"

namespace clox {
namespace runtime {

shared_ptr<Object> LoxFunction::call(
    Interpreter &interpreter,
    const vector<shared_ptr<Object>>
        &arguments) noexcept(noexcept(interpreter.executeBlock({nullptr},
                                                               nullptr))) {
  shared_ptr<Environment> environment =
      std::make_shared<Environment>(closure.lock());
  auto &params = declaration->lambda->params;
  for (auto i = 0; i < params.size(); ++i) {
    environment->define(params.at(i)->lexeme, arguments.at(i));
  }

  decltype(this->call(interpreter, arguments)) return_value = nullptr;

  try {
    interpreter.executeBlock(declaration->lambda->body, environment);
  } catch (const Control &e) {
    return_value = e.data;
  }
  //这里进行修正
  if (isInitializer)
    return closure.lock()->getAt(0, "this");
  return return_value;
}

////////////////////////////////////////////////////////////////////////
//                              LoxClass                              //
////////////////////////////////////////////////////////////////////////

shared_ptr<Object> LoxClass::call(Interpreter &interpreter,
                                  const vector<shared_ptr<Object>> &arguments) {
  auto initializer = findMethod("init");
  auto instance = std::make_shared<LoxInstance>(getClass());
  if (initializer) {
    initializer->bind(instance)->call(interpreter, arguments);
  }
  return instance;
}

int LoxClass::arity() noexcept {
  shared_ptr<LoxFunction> initializer = findMethod("init");
  if (initializer) {
    return initializer->arity();
  }
  return 0;
}
////////////////////////////////////////////////////////////////////////
//                            LoxFunction                             //
////////////////////////////////////////////////////////////////////////

shared_ptr<LoxFunction>
LoxFunction::bind(shared_ptr<LoxInstance> owner) noexcept {
  shared_ptr<Environment> environment =
      std::make_shared<Environment>(closure.lock());
  environment->define("this", owner);
  return std::make_shared<LoxFunction>(declaration, environment, isInitializer);
}
} // namespace runtime
} // namespace clox
