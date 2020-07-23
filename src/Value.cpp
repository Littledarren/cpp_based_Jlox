#include <sstream>

#include "../includes/Value.h"
#include "../includes/RuntimeError.h"



String String::operator+(const Number &value) const
{
    std::ostringstream oss;
    oss<<value;
    return this->value + oss.str();
}

String String::operator+(const Bool &value) const
{
    std::ostringstream oss;
    oss<<value;
    return this->value + oss.str();
}


#include "../includes/Interpreter.h"


