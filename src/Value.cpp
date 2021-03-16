#include "Value.h"

#include <sstream>


#include "Interpreter.h"

String String::operator+(const shared_ptr<Object> &right) const 
{
    String result("");
    if (auto temp_number = std::dynamic_pointer_cast<Number>(right)) 
        result = (*this + *temp_number);
    else if (auto temp_bool = std::dynamic_pointer_cast<Bool>(right))
        result = (*this + *temp_bool);
    else
        result = (this->value + (string)*std::dynamic_pointer_cast<String>(right));
    return result;
}

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


