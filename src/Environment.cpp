#include "../includes/Environment.h"





void Environment::define(const string &name, VALUE_T *value)
{
    values.insert(std::pair<string, VALUE_T*>(name, value));
}
VALUE_T * Environment::get(Token * const  &name)
{

    if (values.count(name->lexeme)) {
        return values.at(name->lexeme);
    }
    throw RuntimeError(name->type, (string("Undefined variable") + name->lexeme).c_str());
}
