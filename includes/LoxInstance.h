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

#include "Object.h"
#include "RuntimeError.h"

struct LoxClass;

struct LoxInstance : public virtual Object,
                     public std::enable_shared_from_this<LoxInstance> {
  using FIELD_TYPE = shared_ptr<Object>;

public:
  LoxInstance(LoxClass *klass) : klass(klass) {}
  virtual ~LoxInstance() = default;
  virtual string toString() const override;
  FIELD_TYPE get(shared_ptr<Token> token);

  void set(shared_ptr<Token> token, FIELD_TYPE value) {
    fields[token->lexeme] = value;
  }

private:
  // 如果持有共享指针，会导致循环引用！
  LoxClass *klass;
  std::map<string, FIELD_TYPE> fields;
};

#endif /* LOXINSTANCE_H */
