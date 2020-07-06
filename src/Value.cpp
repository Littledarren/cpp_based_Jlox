#include <sstream>

#include "../includes/Value.h"
#include "../includes/RuntimeError.h"


//got default parameters
Value::Value(TokenType type, void *literal):
    type(type), literal(literal)
{
    switch(this->type) {
        case STRING:
            this->literal = new string(*(string*)literal);
            break;
        case NUMBER:
            this->literal = new double(*(double*)literal);
            break;
        default:
            //default to nullptr
            break;
    }           
}

Value::Value(const Value& v)
{
    this->type = v.type;
    //deep copy
    switch(this->type) {
        case STRING:
            this->literal = new string(*(string*)v.literal);
            break;
        case NUMBER:
            this->literal = new double(*(double*)v.literal);
            break;
        default:
            //default to nullptr
            break;
    }           
}
Value& Value::operator=(const Value &v)
{
    //赋值方法
    //释放原有的资源，然后使用深拷贝使用新的。
    freeLiteral();
    this->type = v.type;

    //deep copy
    switch(this->type) {
        case STRING:
            this->literal = new string(*(string*)v.literal);
            break;
        case NUMBER:
            this->literal = new double(*(double*)v.literal);
            break;
        default:
            break;
    }           
    return *this;
}

//for MINUS unary
Value Value::operator-() const
{
    Value result(*this);
    *(double*)result.literal = *(double*)result.literal;
    return result;
}
//for BANG
Value Value::operator!() const
{
    Value result(*this);
    if (type == FALSE || (type != TRUE && literal == nullptr)) {
        result.type = TRUE;
        result.literal = nullptr;
    } else {
        result.type = FALSE;
        result.literal =nullptr;
    }
    return result;
}


//binary
Value Value::operator+(const Value &r) const
{
    if (r.type != type) throw RuntimeError(PLUS, "operants should be same");
    //will return a new.

    Value result(*this);

    //type should be double?

    switch (type) {
        case STRING:
            {
                string &r_l = *(string*)result.literal;
                r_l += *(string*)r.literal;
                break;
            }
        case NUMBER:
            {
                double &r_l = *(double*)result.literal;
                r_l += *(double*)r.literal;
                break;
            }
        default:
            throw RuntimeError(PLUS, "operants should be string or number");
    }
    return result;
}
Value Value::operator-(const Value &r) const
{
    return *this + r.operator-();
    //if (r.type != type) throw "operant of - should have same type";

    //Value result(*this);

    //if (type != NUMBER) throw "operator - only for NUMBER";

    //double &r_l = *(double*)result.literal;
    //r_l -= *(double*)r.literal;
    //return result;
}
Value Value::operator*(const Value &r) const
{
    Value result(*this);

    *(double*)result.literal *= *(double*)r.literal;
    return result;

}
Value Value::operator/(const Value &r) const 
{
    Value result(*this);

    *(double*)result.literal /= *(double*)r.literal;
    return result;
}

//comparation
Value Value::operator<(const Value &r) const
{
    const bool greater = *(double*)literal < *(double*)r.literal;
    return greater ? Value(TRUE):Value(FALSE);
}
Value Value::operator==(const Value &r) const 
{
    if (r.type != type) return Value(FALSE);

    if (literal == nullptr && r.literal == nullptr) return Value(TRUE);
    if (literal == nullptr || r.literal == nullptr) return Value(FALSE);


    switch (type) {
        case STRING:
            return (*(string*)literal == *(string*)r.literal) ? Value(TRUE) : Value(FALSE);
        case NUMBER:
            return (*(double*)literal == *(double*)r.literal) ? Value(TRUE) : Value(FALSE);
        case FALSE:
        case TRUE:
            return Value(TRUE);
        default:
            throw "UNEXPECTED ERROR FOR ==";
            break;
    }
}
Value Value::operator!=(const Value &r) const 
{
    return !(*this == r);
}

Value Value::operator<=(const Value &r) const 
{
    return ((*this == r).type == TRUE) || ((*this < r).type == TRUE) ? Value(TRUE) : Value(FALSE);
}

Value Value::operator>(const Value &r) const 
{
    return !(*this <= r);
    return Value((*this <= r).type == TRUE ? FALSE : TRUE);
}
Value Value::operator>=(const Value &r) const 
{
    return !(*this < r);
    return Value((*this < r).type == TRUE ? FALSE : TRUE);
}
//    short-cut not supported ...
//    VALUE operator||(const Value &r) const
//    {
//       if (type == TRUE)  return Value(TRUE);
//    }

void Value::freeLiteral()
{
    if (literal == nullptr) return;
    switch(type) {
        case STRING:
            delete (string*)literal;
            break;
        case NUMBER:
            delete (double*)literal;
            break;
        default:
            break;
    }
}
string Value::toString(Value &v)
{
    std::ostringstream os;

    switch(v.type) {
        case NUMBER:
            os<<*(double*)v.literal;
            break;
        case STRING:
            os<<*(string*)v.literal;
            break;
        case TRUE:
            os<<"true";
            break;
        case FALSE:
            os<<"false";
            break;
        default:
            os<<getNameOfType(v.type);
            break;
    }
    return os.str();
}
