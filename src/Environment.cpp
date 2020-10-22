#include "../includes/Environment.h"


#include "../includes/Token.h"
#include "../includes/RuntimeError.h"

#include <iostream>
using std::endl;
using std::cout;

void Environment::define(const string &name, shared_ptr<const Object> value)
{
    if (values.count(name) != 0) {
        values.at(name) = value;
    } else {
        values.insert(std::pair<const string, shared_ptr<const Object>>(name, value));
    }
}
shared_ptr<const Object> Environment::get(shared_ptr<const Token> name)
{

    if (values.count(name->lexeme) != 0) {
        return values.at(name->lexeme);
    }
    if (enclosing) return enclosing->get(name);
    throw RuntimeError(name, string("Undefined variable") + name->lexeme);
}
void Environment::assign(shared_ptr<const Token> name, shared_ptr<const Object> value)
{

    if (values.count(name->lexeme) != 0) {
        values.at(name->lexeme) = value;
        return;
    }
    if (enclosing) {
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
        cout<<kv.first<<"\t:\t"<<(kv.second)->toString()<<endl;
    }
}
