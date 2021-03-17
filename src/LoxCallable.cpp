

#include "LoxCallable.h"

#include "Interpreter.h"
#include "LoxInstance.h"

shared_ptr<Object> LoxFunction::call(Interpreter &interpreter, const vector<shared_ptr<Object>> &arguments)
{
    shared_ptr<Environment> environment = std::make_shared<Environment>(closure);
    auto& params = declaration->lambda.params;
    for (auto i = 0; i < params.size(); ++i) {
        environment->define(params.at(i)->lexeme, arguments.at(i));
    }

    decltype(this->call(interpreter, arguments)) return_value = nullptr;

    try {
        interpreter.executeBlock(declaration->lambda.body, environment);
    } catch (const Control& e) {
        return_value = e.data;
    }  catch (const runtime_error&e) {
        std::cerr<<e.what()<<std::endl;
    }
    return return_value;
}

shared_ptr<Object> LoxClass::call(Interpreter &interpreter, const vector<shared_ptr<Object>> &arguments)
{
    return std::make_shared<LoxInstance>(*this);
}
int LoxClass::arity()
{
    return 0;
}

