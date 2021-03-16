#include <exception>
#include "Interpreter.h"
#include "main.h"
#include "LoxCallable.h"

#include <iostream>
using std::cout;
using std::cin;



Interpreter::Interpreter():environment(new Environment())
{
    environment->define("clock", std::make_shared<Clock>());
}


void Interpreter::interprete(vector<StmtPtr> statements)
{
    try {
        for (auto p : statements) {
            execute(p);
        }
    } catch (const RuntimeError &e) {
        runtimeError(e);
    }
}

RETURN_TYPE Interpreter::interprete(shared_ptr<Expr> expr)
{
    try {
        return evaluate(expr);
    } catch (const RuntimeError &e) {
        runtimeError(e);
    } catch (...) {
        std::cout<<"other exception found"<<std::endl;
    }
    return nullptr;
}


RETURN_TYPE Interpreter::evaluate(shared_ptr<Expr> expr)
{
    if (expr)
        return expr->accept(*this);
    else 
        throw RuntimeError(std::make_shared<Token>(NUL, "", nullptr, -1), "expr is nullptr!!");
}
void Interpreter::execute(shared_ptr<Stmt> stmt)
{
    if (stmt)
        stmt->accept(*this);
    else
        throw RuntimeError(std::make_shared<Token>(NUL, "", nullptr, -1), "stmt is nullptr!!");
}
void Interpreter::executeBlock(vector<shared_ptr<Stmt>> stmts, shared_ptr<Environment> environment)
{
    shared_ptr<Environment> previous = this->environment;
    try {
        this->environment = (environment);
        for (auto ptr : stmts) {
            execute(ptr);
        }
    } catch (...) {
        this->environment = previous;
        throw;
    }
    this->environment = previous;
}


////////////////////////////////////////////////////////////////////////
//                               visit                                //
////////////////////////////////////////////////////////////////////////


RETURN_TYPE Interpreter::visit(const Assign &expr)
{
    RETURN_TYPE value = evaluate(expr.value);
    environment->assign(expr.name, value);
    //may be clone???
    return value;
}

RETURN_TYPE Interpreter::visit(const Logical &expr)
{
    RETURN_TYPE left = evaluate(expr.left);
    if (expr.op->type == AND) {
        if (!left || !left->isTrue()) return left;
    } else {
        if (left && left->isTrue()) return left;
    }
    return evaluate(expr.right);
}

RETURN_TYPE Interpreter::visit(const Call &expr)
{
    RETURN_TYPE callee = (evaluate(expr.callee));

    vector<RETURN_TYPE> arguments;
    for (auto p : expr.arguments) {
        arguments.push_back(evaluate(p));
    }

    shared_ptr<Callable> function = std::dynamic_pointer_cast<Callable>(callee);
    if (!function) {
        throw RuntimeError(expr.paren, "can only call functions and classes");
    }
    if (arguments.size() != function->arity()) {
        ostringstream oss;
        oss<<"Expected "<<function->arity() << "arguments but got"<<
            arguments.size()<<".";
        throw RuntimeError(expr.paren, oss.str());
    }

    //return new Object((*callee)(arguments));
    return function->call(*this, arguments);
}

RETURN_TYPE Interpreter::visit(const Ternary &expr) 
{

    RETURN_TYPE cond = evaluate(expr.condition);

    if (cond && cond->isTrue()) {
        return evaluate(expr.if_yes);
    } else {
        return evaluate(expr.if_no);
    } 
}
RETURN_TYPE Interpreter::visit(const Binary &expr) 
{
    //this will creat a new ..
    RETURN_TYPE left = evaluate(expr.left);
    RETURN_TYPE right = evaluate(expr.right);


    bool is_string = !!std::dynamic_pointer_cast<String>(left);

    RETURN_TYPE result = nullptr;

    switch(expr.op->type)
    {
        case PLUS:
            checkStringOrNumber(expr.op, left, right);
            //字符串可以跟其他类型+
            //要么都是数字，要么左边是字符串，没有其他可能
            if (is_string) {
                if (auto temp_number = std::dynamic_pointer_cast<Number>(right)) 
                    result = std::make_shared<String>(*std::dynamic_pointer_cast<String>(left) + *temp_number);
                else if (auto temp_bool = std::dynamic_pointer_cast<Bool>(right))
                    result = std::make_shared<String>(*std::dynamic_pointer_cast<String>(left) + *temp_bool);
                else
                    result = std::make_shared<String>((string)*std::dynamic_pointer_cast<String>(left) + (string)*std::dynamic_pointer_cast<String>(right));
            } else
                result = std::make_shared<Number>(*std::dynamic_pointer_cast<Number>(left) + *std::dynamic_pointer_cast<Number>(right));
            break;
        case MINUS:
            chechNumber(expr.op, left, right);
            result = std::make_shared<Number>(*std::dynamic_pointer_cast<Number>(left) - *std::dynamic_pointer_cast<Number>(right));
            break;
        case STAR:
            chechNumber(expr.op, left, right);
            result = std::make_shared<Number>(*std::dynamic_pointer_cast<Number>(left) * *std::dynamic_pointer_cast<Number>(right));
            break;
        case SLASH:
            chechNumber(expr.op, left, right);
            result = std::make_shared<Number>(*std::dynamic_pointer_cast<Number>(left) / *std::dynamic_pointer_cast<Number>(right));
            break;
        case COMMA:
            return right;
            break;
        case GREATER:
            chechNumber(expr.op, left, right);
            result = std::make_shared<Bool>(*std::dynamic_pointer_cast<Number>(left) > *std::dynamic_pointer_cast<Number>(right));
            break;
        case GREATER_EQUAL:
            chechNumber(expr.op, left, right);
            result = std::make_shared<Bool>(*std::dynamic_pointer_cast<Number>(left) >= *std::dynamic_pointer_cast<Number>(right));
            break;
        case LESS:
            chechNumber(expr.op, left, right);
            result = std::make_shared<Bool>(*std::dynamic_pointer_cast<Number>(left) < *std::dynamic_pointer_cast<Number>(right));
            break;
        case LESS_EQUAL:
            chechNumber(expr.op, left, right);
            result = std::make_shared<Bool>(*std::dynamic_pointer_cast<Number>(left) <= *std::dynamic_pointer_cast<Number>(right));
            break;
        case EQUAL_EQUAL:
            result = std::make_shared<Bool>(*left==*right);
            break;
        case BANG_EQUAL:
            result = std::make_shared<Bool>(!(*left==*right));
            break;
        default:
            throw string("ERROR UNKONE OP") + expr.op->lexeme;
    }
    return result;
}
RETURN_TYPE Interpreter::visit(const Unary &expr) 
{
    RETURN_TYPE temp = evaluate(expr.right);
    RETURN_TYPE result = nullptr;
    switch(expr.op->type) {
        case MINUS:
            result = std::make_shared<Number>(-*std::dynamic_pointer_cast<Number>(temp));
            break;
        case BANG:
            result = std::make_shared<Bool>(!*std::dynamic_pointer_cast<Bool>(temp));
            break;
        default:
            throw RuntimeError(expr.op, "r u kidding me with a wrong unary op?");
    }
    return result;
}
RETURN_TYPE Interpreter::visit(const Grouping &expr) 
{
    return evaluate(expr.expr);
}
RETURN_TYPE Interpreter::visit(const Literal &expr) 
{
    return expr.value;
}
RETURN_TYPE  Interpreter::visit(const Variable &expr) 
{
    return environment->get(expr.name);
}

RETURN_TYPE Interpreter::visit(const Lambda &expr) 
{
    return std::make_shared<LoxFunction>(Function(nullptr, expr.params, expr.body), environment);
}
void  Interpreter::visit(const Expression &stmt) 
{
    evaluate(stmt.expr);
}
void  Interpreter::visit(const Print &stmt) 
{
   RETURN_TYPE value = evaluate(stmt.expr);
   if (value)
       std::cout<<(value)->toString()<<endl;
   else 
       std::cout<<"Nil"<<endl;
}

void Interpreter::visit(const Var &stmt)
{
    RETURN_TYPE value = nullptr;
    if (stmt.initializer != nullptr) {
        value = evaluate(stmt.initializer);
    }
    this->environment->define(stmt.name->lexeme, value);
}
void Interpreter::visit(const Block &stmt)
{
    executeBlock(stmt.statements, std::make_shared<Environment>(environment));
}

void Interpreter::visit(const If &stmt)
{
    RETURN_TYPE cond = evaluate(stmt.condition);
    if (cond && cond->isTrue()) {
        execute(stmt.thenBranch);
    } else if(stmt.elseBranch) {
        execute(stmt.elseBranch);
    } 
}

void Interpreter::printEnvironment()
{
    shared_ptr<Environment> temp = this->environment;
    int scopeCount = 0;
    while (temp) {
        cout<<"======SCOPE:"<<scopeCount<<"======"<<endl;
        temp->print();
        temp = temp->enclosing;
    }
}

void Interpreter::visit(const While &stmt) 
{
    RETURN_TYPE p = nullptr;
    while ((p = evaluate(stmt.condition)) && p->isTrue()) {
        execute(stmt.body);
    }
}
void Interpreter::visit(const Function &func) 
{
    environment->define(func.name->lexeme, std::make_shared<LoxFunction>(func, this->environment));
}


void Interpreter::visit(const Return&stmt) 
{
    RETURN_TYPE value = nullptr;
    if (stmt.value) {
        value = evaluate(stmt.value);
    }
    throw Control(value, stmt.name->type);
}
void Interpreter::checkStringOrNumber(shared_ptr<Token>op, RETURN_TYPE l, RETURN_TYPE r)
{
    if (std::dynamic_pointer_cast<Number>(l) && std::dynamic_pointer_cast<Number>(r)) return;
    if (std::dynamic_pointer_cast<String>(l)) return;
    throw RuntimeError(op, "operands should be numbers or strings ");
}
void Interpreter::chechNumber(shared_ptr<Token>op, shared_ptr<Object> v)
{
    if (std::dynamic_pointer_cast<Number>(v)) return;
    throw RuntimeError(op, "Operand must be a number");
}
void Interpreter::chechNumber(shared_ptr<Token>op, shared_ptr<Object> l, shared_ptr<Object> r)
{
    if (std::dynamic_pointer_cast<Number>(l) && std::dynamic_pointer_cast<Number>(r)) return;
    throw RuntimeError(op, "operands should be numbers");
}
