
#include "../includes/Object.h"

#include "../includes/Value.h"

bool Object::operator==(const Object &o) const { return this == &o; }

#include <sstream>

shared_ptr<Object> Object::clone() const { return nullptr; }

string Object::toString() const {
  std::ostringstream oss;
  oss << "address:" << std::ios::hex << reinterpret_cast<long long>(this)
      << std::ios::dec;
  return oss.str();
}

bool Object::isTrue() const {
  //能调用的肯定不是nullptr
  return true;
}
