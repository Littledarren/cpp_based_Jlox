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

#include "ErrorReporting.h"
#include "Object.h"

namespace clox {
namespace runtime {
struct LoxClass;
}
namespace value {
// Token.h在error reporting中被包含了。。。
using runtime::LoxClass;
using token::Token;

struct LoxInstance : public virtual Object,
                     public std::enable_shared_from_this<LoxInstance> {
  using FIELD_TYPE = shared_ptr<Object>;

public:
  LoxInstance(const shared_ptr<LoxClass> &klass = nullptr) noexcept
      : klass(klass) {}

  virtual ~LoxInstance() noexcept = default;
  virtual string toString() const noexcept override;
  FIELD_TYPE get(shared_ptr<Token> token) noexcept(false);

  void set(shared_ptr<Token> token, FIELD_TYPE value) noexcept {
    fields[token->lexeme] = value;
  }

  shared_ptr<LoxClass> getClass();

private:
  // 如果持有共享指针，会导致循环引用！
  // 但是不持有共享指针，超出类的作用域就over
  // 如果为nullptr,说明自身是LoxClass
  shared_ptr<LoxClass> klass;
  std::map<string, FIELD_TYPE> fields;
};

} // namespace value
} // namespace clox

#endif /* LOXINSTANCE_H */
