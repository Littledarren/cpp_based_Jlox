#include "../includes/Environment.h"


#include "../includes/Token.h"
#include "../includes/RuntimeError.h"

#include <iostream>
using std::endl;
using std::cout;

void Environment::define(const string &name, const Object *value)
{
    if (values.count(name) != 0) {
        delete values.at(name);
        values.at(name) = value;
    } else {
        values.insert(std::pair<const string, const Object*>(name, value));
    }
}
const Object* Environment::get(const Token *name)
{

    if (values.count(name->lexeme) != 0) {
        return values.at(name->lexeme);
    }
    if (enclosing != nullptr) return enclosing->get(name);
    throw RuntimeError(name, string("Undefined variable") + name->lexeme);
}
void Environment::assign(const Token *name, const Object *value)
{

    if (values.count(name->lexeme) != 0) {
        values.at(name->lexeme) = value;
        return;
    }
    if (enclosing != nullptr) {
        enclosing->assign(name, value);
        return;
    }
    throw RuntimeError(name, string("Undefined variable") + name->lexeme);
}
//const std::map<string, Object*>& Environment::getObjects() const
//{
//    return this->values;
//}
void Environment::print() const
{
    for (auto kv : values) {
        cout<<kv.first<<"\t:\t"<<Object::toString(kv.second)<<endl;
    }
}
