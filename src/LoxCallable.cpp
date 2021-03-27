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

////////////////////////////////////////////////////////////////////////
//                              LoxClass                              //
////////////////////////////////////////////////////////////////////////

shared_ptr<Object> LoxClass::call(Interpreter &interpreter,
                                  const vector<shared_ptr<Object>> &arguments) {
  auto initializer = findMethod("init");
  auto instance = std::make_shared<LoxInstance>(
      std::dynamic_pointer_cast<LoxClass>(shared_from_this()));
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

shared_ptr<Object>
LoxFunction::call(Interpreter &interpreter,
                  const vector<shared_ptr<Object>> &arguments) noexcept(false) {
  //闭包必须强引用定义它的那个环境
  //所以会产生循环依赖
  //通过hold_closure保证生命周期，只通过weak_ptr访问
  //只有return的时候，才会hold住闭包
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
  if (declaration->type == FunctionType::INITIALIZER)
    return closure.lock()->getAt(0, "this");
  return return_value;
}

shared_ptr<LoxFunction>
LoxFunction::bind(shared_ptr<LoxInstance> owner) noexcept {
  shared_ptr<Environment> environment =
      std::make_shared<Environment>(closure.lock());
  environment->define("this", owner);
  //这里。需要保存一个对象的闭包
  auto instance_func = std::make_shared<LoxFunction>(declaration, environment);
  return instance_func->hold();
}
} // namespace runtime
} // namespace clox
