/*================================================================
*
*
*   FileName: Object.h
*   Author: DarrenHuang
*   Create Time: 2020/07/22  20:51(Wednesday)
*   Description:
*
================================================================*/
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <memory>
#include <string>

namespace clox {

using std::shared_ptr;
using std::string;
namespace value {

struct Object {

  virtual bool operator==(const Object &o) const noexcept;
  virtual string toString() const noexcept;
  virtual shared_ptr<Object> clone() const noexcept;
  virtual bool isTrue() const noexcept;
  virtual ~Object() noexcept {}
};
} // namespace value
} // namespace clox

#endif
