#include "../includes/Environment.h"


#include "../includes/Token.h"
#include "../includes/RuntimeError.h"


void Environment::define(const string &name, Value *value)
{
    if (value == nullptr) value = new Value(NIL);
    if (values.count(name)) {
        delete values.at(name);

        values.at(name) = value;
    } else {
        values.insert(std::pair<string, Value*>(name, value));
    }
}
Value * Environment::get(Token *name)
{

    if (values.count(name->lexeme) != 0) {
        return values.at(name->lexeme);
    }
    throw RuntimeError(name->type, (string("Undefined variable") + name->lexeme).c_str());
}
void Environment::assign(Token *name, Value *value)
{

    if (values.count(name->lexeme) != 0) {
        values.at(name->lexeme) = value;
        return;
    }
    throw RuntimeError(name->type, (string("Undefined variable") + name->lexeme).c_str());
}
