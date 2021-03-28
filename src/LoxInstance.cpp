#include "LoxInstance.h"
#include "LoxCallable.h"

namespace clox {
namespace value {

using namespace runtime;
using namespace compiling;
using namespace error;

string LoxInstance::toString() const noexcept {
  return const_cast<LoxInstance *>(this)->getClass()->name + "  instance";
}

LoxInstance::FIELD_TYPE
LoxInstance::get(shared_ptr<Token> token) noexcept(false) {
  // fields
  if (fields.find(token->lexeme) != fields.end())
    return fields.at(token->lexeme);

  // methods
  auto method = getClass()->findMethod(token->lexeme);
  //方法是否应该持有对象的强引用？
  //如果是this,那就是弱引用，可能出现对象释放了，但方法还活着，比如内部。。呃
  if (method) {
    if (!klass && method->declaration->type == FunctionType::STATIC_METHOD)
      return method->bind(getClass());
    else if (klass)
      return method->bind(shared_from_this());

    throw RuntimeError(token, "invalid method called");
  }

  throw RuntimeError(token, "Undefined property '" + token->lexeme + "'");
}

shared_ptr<LoxClass> LoxInstance::getClass() {
  if (klass)
    return klass;
  return std::dynamic_pointer_cast<LoxClass>(shared_from_this());
}
} // namespace value
} // namespace clox
