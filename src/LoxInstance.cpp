#include "LoxInstance.h"
#include "LoxCallable.h"

namespace clox {
namespace value {

using namespace runtime;
using namespace compiling;
using namespace error;

string LoxInstance::toString() const { return klass->name + "  instance"; }

LoxInstance::FIELD_TYPE LoxInstance::get(shared_ptr<Token> token) {
  if (fields.find(token->lexeme) != fields.end())
    return fields.at(token->lexeme);
  auto method = klass->findMethod(token->lexeme);
  //方法是否应该持有对象的强引用？
  //如果是this,那就是弱引用，可能出现对象释放了，但方法还活着，比如内部。。呃
  if (method) {
    if (this == klass &&
        method->declaration->type == FunctionType::STATIC_METHOD)
      return method->bind(shared_from_this());
    else if (this != klass)
      return method->bind(shared_from_this());

    throw RuntimeError(token, "invalid method called");
  }

  throw RuntimeError(token, "Undefined property '" + token->lexeme + "'");
}
} // namespace value
} // namespace clox
