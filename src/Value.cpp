#include "Value.h"

#include <sstream>

namespace clox {
namespace value {
////////////////////////////////////////////////////////////////////////
//                               Number                               //
////////////////////////////////////////////////////////////////////////

bool Number::operator==(const Object &r) const noexcept {
  const Number *rp = dynamic_cast<const Number *>(&r);
  if (!rp)
    return false;
  return value == rp->value;
}

string Number::toString() const noexcept {
  std::ostringstream oss;
  oss << value;
  return oss.str();
}
////////////////////////////////////////////////////////////////////////
//                                Bool                                //
////////////////////////////////////////////////////////////////////////

bool Bool::operator==(const Object &r) const noexcept {
  const Bool *rp = dynamic_cast<const Bool *>(&r);
  if (rp == nullptr)
    return false;
  return value == rp->value;
}
////////////////////////////////////////////////////////////////////////
//                              String                                //
////////////////////////////////////////////////////////////////////////

bool String::operator==(const Object &r) const noexcept {
  const String *rp = dynamic_cast<const String *>(&r);
  if (rp == nullptr)
    return false;
  return value == rp->value;
}

String String::operator+(const Object &obj) const noexcept {
  return value + obj.toString();
}

shared_ptr<String> StringAdd(const shared_ptr<String> &str,
                             const shared_ptr<Object> &obj) noexcept {
  if (obj)
    return std::make_shared<String>(*str + *obj);
  else
    return std::make_shared<String>(*str + String("nil"));
}
} // namespace value
} // namespace clox
