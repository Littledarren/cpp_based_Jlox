
#include "../includes/Object.h"

#include "../includes/Value.h"


bool Object::operator==(const Object &o) const
{
    return this == &o;
}


#include <sstream>


string Object::toString() const
{
    std::ostringstream oss;
    oss<<"address:"<<std::ios::hex<<reinterpret_cast<long long>(this)<<std::ios::dec;
    return oss.str(); 
}
Object* Object::clone(const Object* o)
{
    if (o == nullptr) return nullptr;
    if (typeid(*o) == typeid(Number)) {
        return new Number(dynamic_cast<const Number&>(*o));
    }
    
    if (typeid(*o) == typeid(String)) {
        return new String(dynamic_cast<const String&>(*o));
    }
    if (typeid(*o) == typeid(Bool)) {
        return new Bool(dynamic_cast<const Bool&>(*o));
    }
    return new Object(*o);
}


string Object::toString(const Object* o)
{
    if (o == nullptr) return "nil";
    
    return o->toString();
}
