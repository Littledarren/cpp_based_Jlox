

#include "../includes/main.h"
#include "../includes/Interpreter.h"


#include <sstream>

//got default parameters
VALUE_T::VALUE_T(TokenType type, void *literal):
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

VALUE_T::VALUE_T(const VALUE_T& v)
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
VALUE_T& VALUE_T::operator=(const VALUE_T &v)
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
VALUE_T VALUE_T::operator-() const
{
    VALUE_T result(*this);
    *(double*)result.literal = *(double*)result.literal;
    return result;
}
//for BANG
VALUE_T VALUE_T::operator!() const
{
    VALUE_T result(*this);
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
VALUE_T VALUE_T::operator+(const VALUE_T &r) const
{
    if (r.type != type) throw RuntimeError(PLUS, "operants should be same");
    //will return a new.

    VALUE_T result(*this);

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
VALUE_T VALUE_T::operator-(const VALUE_T &r) const
{
    return *this + r.operator-();
    //if (r.type != type) throw "operant of - should have same type";

    //VALUE_T result(*this);

    //if (type != NUMBER) throw "operator - only for NUMBER";

    //double &r_l = *(double*)result.literal;
    //r_l -= *(double*)r.literal;
    //return result;
}
VALUE_T VALUE_T::operator*(const VALUE_T &r) const
{
    VALUE_T result(*this);

    *(double*)result.literal *= *(double*)r.literal;
    return result;

}
VALUE_T VALUE_T::operator/(const VALUE_T &r) const 
{
    VALUE_T result(*this);

    *(double*)result.literal /= *(double*)r.literal;
    return result;
}

//comparation
VALUE_T VALUE_T::operator<(const VALUE_T &r) const
{
    const bool greater = *(double*)literal < *(double*)r.literal;
    return greater ? VALUE_T(TRUE):VALUE_T(FALSE);
}
VALUE_T VALUE_T::operator==(const VALUE_T &r) const 
{
    if (r.type != type) return VALUE_T(FALSE);

    if (literal == nullptr && r.literal == nullptr) return VALUE_T(TRUE);
    if (literal == nullptr || r.literal == nullptr) return VALUE_T(FALSE);


    switch (type) {
        case STRING:
            return (*(string*)literal == *(string*)r.literal) ? VALUE_T(TRUE) : VALUE_T(FALSE);
        case NUMBER:
            return (*(double*)literal == *(double*)r.literal) ? VALUE_T(TRUE) : VALUE_T(FALSE);
        case FALSE:
        case TRUE:
            return VALUE_T(TRUE);
        default:
            throw "UNEXPECTED ERROR FOR ==";
            break;
    }
}
VALUE_T VALUE_T::operator!=(const VALUE_T &r) const 
{
    return !(*this == r);
}

VALUE_T VALUE_T::operator<=(const VALUE_T &r) const 
{
    return ((*this == r).type == TRUE) || ((*this < r).type == TRUE) ? VALUE_T(TRUE) : VALUE_T(FALSE);
}

VALUE_T VALUE_T::operator>(const VALUE_T &r) const 
{
    return !(*this <= r);
    return VALUE_T((*this <= r).type == TRUE ? FALSE : TRUE);
}
VALUE_T VALUE_T::operator>=(const VALUE_T &r) const 
{
    return !(*this < r);
    return VALUE_T((*this < r).type == TRUE ? FALSE : TRUE);
}
//    short-cut not supported ...
//    VALUE operator||(const VALUE_T &r) const
//    {
//       if (type == TRUE)  return VALUE_T(TRUE);
//    }

void VALUE_T::freeLiteral()
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
void VALUE_T::checkStringOrNumber(TokenType op, const VALUE_T &l, const VALUE_T &r)
{
    if (l.type == NUMBER && r.type == NUMBER) return;
    if (l.type == STRING && r.type == STRING) return;
    throw RuntimeError(op, "operands should be numbers");
}
void VALUE_T::chechNumber(TokenType op, const VALUE_T &v)
{
    if (v.type != NUMBER) throw RuntimeError(op, "Operand must be a number");
}
void VALUE_T::chechNumber(TokenType op, const VALUE_T &l, const  VALUE_T &r)
{
    if (l.type != NUMBER || r.type != NUMBER) throw RuntimeError(op, "operands should be numbers");
}
string VALUE_T::toString(VALUE_T &v)
{
    ostringstream os;
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


VALUE_T Interpreter::interprete(Expr *expr)
{
    try {
        VALUE_T *temp = (VALUE_T*)evaluate(expr);
        //because i define deep copy 
        //so this is allowed;
        VALUE_T result(*temp);
        delete temp;
        //because defined deep copy so allowed
        return result;
    } catch (const char *str) {
        std::cout<<str<<std::endl;
    } catch (const RuntimeError &e) {
        runtimeError(e);
    }
    return VALUE_T();
}


void* Interpreter::evaluate(Expr *expr)
{
    return expr->accept(this);
}
void* Interpreter::visitTernaryExpr(Ternary *expr) 
{
    return nullptr;
}
void* Interpreter::visitBinaryExpr(Binary *expr) 
{
    //this will creat a new ..
    VALUE_T *left = (VALUE_T*)evaluate(expr->left);
    VALUE_T *right = (VALUE_T*)evaluate(expr->right);
    VALUE_T *result = new VALUE_T;
    switch(expr->op->type)
    {
        case PLUS:
            VALUE_T::checkStringOrNumber(PLUS, *left, *right);
            *result =  *left + *right;
            break;
        case MINUS:
            VALUE_T::chechNumber(MINUS, *left, *right);
            *result = *left - *right;
            break;
        case STAR:
            VALUE_T::chechNumber(MINUS, *left, *right);
            *result =  *left * *right;
            break;
        case SLASH:
            VALUE_T::chechNumber(MINUS, *left, *right);
            *result = *left / *right;
            break;
        case COMMA:
            *result = *right;
            break;
        case GREATER:
            VALUE_T::chechNumber(MINUS, *left, *right);
            *result = *left > *right;
            break;
        case GREATER_EQUAL:
            VALUE_T::chechNumber(MINUS, *left, *right);
            *result = *left >= *right;
            break;
        case LESS:
            VALUE_T::chechNumber(MINUS, *left, *right);
            *result = *left < *right;
            break;
        case LESS_EQUAL:
            VALUE_T::chechNumber(MINUS, *left, *right);
            *result = *left <= *right;
            break;
        case EQUAL_EQUAL:
            *result = *left == *right;
            break;
        case BANG_EQUAL:
            *result = *left != *right;
            break;
        default:
            throw string("ERROR UNKONE OP") + expr->op->lexeme;
    }
    delete left;
    delete right;
    return result;
}
void* Interpreter::visitUnaryExpr(Unary *expr) 
{
    VALUE_T *temp = (VALUE_T*)evaluate(expr->right);
    VALUE_T *result = nullptr;
    switch(expr->op->type) {
        case MINUS:
            result = new VALUE_T(-*temp);
            break;
        case BANG:
            result = new VALUE_T(!*temp);
            break;
        default:
            throw std::runtime_error("r u kidding me with a wrong unary op?");
    }
    delete temp;
    return result;
}
void* Interpreter::visitGroupingExpr(Grouping *expr) 
{
    return evaluate(expr->expr);
}
void* Interpreter::visitLiteralExpr(Literal *expr) 
{
    return new VALUE_T(*(expr->type), expr->value);
}




