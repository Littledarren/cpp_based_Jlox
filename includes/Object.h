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

  virtual bool operator==(const Object &o) const;
  virtual string toString() const;
  virtual shared_ptr<Object> clone() const;
  virtual bool isTrue() const;
  virtual ~Object() {}
};
} // namespace value
} // namespace clox

#endif
