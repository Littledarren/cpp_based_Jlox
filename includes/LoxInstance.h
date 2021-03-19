/*================================================================
*
*
*   FileName: LoxInstance.h
*   Author: DarrenHuang
*   Create Time: 2021/03/17  19:21(星期三)
*   Description:
*
================================================================*/
#ifndef LOXINSTANCE_H
#define LOXINSTANCE_H

#include <map>
#include <memory>

#include "LoxCallable.h"
#include "Object.h"
#include "RuntimeError.h"

struct LoxInstance : public virtual Object,
                     public std::enable_shared_from_this<LoxInstance> {
  using FIELD_TYPE = shared_ptr<Object>;

public:
  LoxInstance(LoxClass &klass) : klass(klass) {}
  virtual ~LoxInstance() = default;
  virtual string toString() const override { return klass.name + "  instance"; }
  FIELD_TYPE get(shared_ptr<Token> token) {
    if (fields.find(token->lexeme) != fields.end())
      return fields.at(token->lexeme);
    auto method = klass.findMethod(token->lexeme);
    //方法是否应该持有对象的强引用？
    //如果是this,那就是弱引用，可能出现对象释放了，但方法还活着，比如内部。。呃
    if (method)
      return method->bind(shared_from_this());

    throw RuntimeError(token, "Undefined property '" + token->lexeme + "'");
  }

  void set(shared_ptr<Token> token, FIELD_TYPE value) {
    fields[token->lexeme] = value;
  }

private:
  LoxClass &klass;
  std::map<string, FIELD_TYPE> fields;
};

#endif /* LOXINSTANCE_H */
