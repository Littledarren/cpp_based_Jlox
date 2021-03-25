#include "Object.h"

#include <sstream>

namespace clox {
namespace value {

bool Object::operator==(const Object &o) const noexcept { return this == &o; }

shared_ptr<Object> Object::clone() const noexcept { return nullptr; }

string Object::toString() const noexcept {
  std::ostringstream oss;
  oss << "address:" << std::ios::hex << reinterpret_cast<long long>(this)
      << std::ios::dec;
  return oss.str();
}

bool Object::isTrue() const noexcept {
  //能调用的肯定不是nullptr
  return true;
}
} // namespace value
} // namespace clox
