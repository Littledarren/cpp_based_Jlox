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
  //可能会传入this指针。。所以不能是自身的引用？
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

} // namespace value
} // namespace clox

#endif /* LOXINSTANCE_H */
