#include "../includes/Environment.h"


#include "../includes/Token.h"
#include "../includes/RuntimeError.h"

#include <iostream>
using std::endl;
using std::cout;

void Environment::define(const string &name, Value *value)
{
    if (value == nullptr) value = new Value(NIL);
    if (values.count(name) != 0) {
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
    if (enclosing != nullptr) return enclosing->get(name);
    throw RuntimeError(name->type, (string("Undefined variable") + name->lexeme).c_str());
}
void Environment::assign(Token *name, Value *value)
{

    if (values.count(name->lexeme) != 0) {
        values.at(name->lexeme) = value;
        return;
    }
    if (enclosing != nullptr) {
        enclosing->assign(name, value);
        return;
    }    throw RuntimeError(name->type, (string("Undefined variable") + name->lexeme).c_str());
}
//const std::map<string, Value*>& Environment::getValues() const
//{
//    return this->values;
//}
void Environment::print() const
{
    for (auto kv : values) {
        cout<<kv.first<<"\t:\t"<<Value::toString(*kv.second)<<endl;
    }
}
