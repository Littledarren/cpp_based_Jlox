#include "Value.h"

#include <sstream>
#include <stdexcept>

namespace clox {
namespace value {
////////////////////////////////////////////////////////////////////////
//                               Number                               //
////////////////////////////////////////////////////////////////////////

using std::runtime_error;
bool Number::operator==(const Object &r) const {
  const Number *rp = dynamic_cast<const Number *>(&r);
  if (rp == nullptr)
    throw runtime_error("Number class should not be compared with nullptr");
  return value == rp->value;
}

string Number::toString() const {
  std::ostringstream oss;
  oss << value;
  return oss.str();
}
////////////////////////////////////////////////////////////////////////
//                                Bool                                //
////////////////////////////////////////////////////////////////////////

bool Bool::operator==(const Object &r) const {
  const Bool *rp = dynamic_cast<const Bool *>(&r);
  if (rp == nullptr)
    return false;
  return value == rp->value;
}
////////////////////////////////////////////////////////////////////////
//                              String                                //
////////////////////////////////////////////////////////////////////////

bool String::operator==(const Object &r) const {
  const String *rp = dynamic_cast<const String *>(&r);
  if (rp == nullptr)
    return false;
  return value == rp->value;
}

String String::operator+(const Object &obj) const {
  return value + obj.toString();
}

shared_ptr<String> StringAdd(const shared_ptr<String> &str,
                             const shared_ptr<Object> &obj) {
  if (obj)
    return std::make_shared<String>(*str + *obj);
  else
    return std::make_shared<String>(*str + String("Nil"));
}
} // namespace value
} // namespace clox
