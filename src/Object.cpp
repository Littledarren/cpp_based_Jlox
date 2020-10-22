
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

