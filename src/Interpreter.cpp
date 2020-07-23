#include "../includes/Interpreter.h"
#include "../includes/main.h"




void Interpreter::interprete(vector<Stmt*> statements)
{
    try {
        for (auto p : statements) {
            execute(p);
        }
    } catch (const RuntimeError &e) {
        runtimeError(e);
    }
}

const Object* Interpreter::interprete(const Expr *expr)
{
    try {
        return evaluate(expr);
    } catch (const char *str) {
        std::cout<<str<<std::endl;
    } catch (const RuntimeError &e) {
        runtimeError(e);
    }
    return nullptr;
}


const Object* Interpreter::evaluate(const Expr *expr)
{
    if (expr != nullptr)
        return expr->accept(this);
    else 
        return nullptr;
}
void Interpreter::execute(const Stmt *stmt)
{
    if (stmt != nullptr)
        stmt->accept(this);
}
void Interpreter::executeBlock(vector<Stmt*> stmts, Environment *environment)
{
    Environment *previous = this->environment;
    try {
        this->environment = environment;
        for (const Stmt* statement : stmts) {
            execute(statement);
        }
    } catch (const exception &e) {
        cout<<e.what()<<endl;
    }
    delete this->environment;
    this->environment = previous;
}
const Object* Interpreter::visitTernaryExpr(const Ternary *expr) 
{
    return nullptr;
}
const Object* Interpreter::visitBinaryExpr(const Binary *expr) 
{
    //this will creat a new ..
    const Object &left = *evaluate(expr->left);
    const Object &right = *evaluate(expr->right);


    bool is_string = typeid(left) == typeid(String);

    const Object *result = nullptr;
    switch(expr->op->type)
    {
        case PLUS:
            checkStringOrNumber(expr->op, left, right);
            if (is_string) {
                if (typeid(right) == typeid(Number))
                    result = new String(dynamic_cast<const String&>(left) + dynamic_cast<const Number&>(right));
                else if (typeid(right) == typeid(Bool))
                    result = new String(dynamic_cast<const String&>(left) + dynamic_cast<const Bool&>(right));
                else
                    result = new String((const string)dynamic_cast<const String&>(left) + (const string)dynamic_cast<const String&>(right));

            } else
                result = new Number(dynamic_cast<const Number&>(left) + dynamic_cast<const Number&>(right));
            break;
        case MINUS:
            chechNumber(expr->op, left, right);
            result = new Number(dynamic_cast<const Number&>(left) - dynamic_cast<const Number&>(right));
            break;
        case STAR:
            chechNumber(expr->op, left, right);
            result = new Number(dynamic_cast<const Number&>(left) * dynamic_cast<const Number&>(right));
            break;
        case SLASH:
            chechNumber(expr->op, left, right);
            result = new Number(dynamic_cast<const Number&>(left) / dynamic_cast<const Number&>(right));
            break;
        case COMMA:
            delete &left;
            return &right;
            break;
        case GREATER:
            chechNumber(expr->op, left, right);
            result = new Bool(dynamic_cast<const Number&>(left) > dynamic_cast<const Number&>(right));
            break;
        case GREATER_EQUAL:
            chechNumber(expr->op, left, right);
            result = new Bool(dynamic_cast<const Number&>(left) >= dynamic_cast<const Number&>(right));
            break;
        case LESS:
            chechNumber(expr->op, left, right);
            result = new Bool(dynamic_cast<const Number&>(left) < dynamic_cast<const Number&>(right));
            break;
        case LESS_EQUAL:
            chechNumber(expr->op, left, right);
            result = new Bool(dynamic_cast<const Number&>(left) <= dynamic_cast<const Number&>(right));
            break;
        case EQUAL_EQUAL:
            result = new Bool(left == right);
            break;
        case BANG_EQUAL:
            result = new Bool(!(left == right));
            break;
        default:
            throw string("ERROR UNKONE OP") + expr->op->lexeme;
    }
    delete &left;
    delete &right;
    return result;
}
const Object* Interpreter::visitUnaryExpr(const Unary *expr) 
{
    const Object* temp = evaluate(expr->right);
    const Object* result = nullptr;
    switch(expr->op->type) {
        case MINUS:
            result = new Number(-dynamic_cast<const Number&>(*temp));
            break;
        case BANG:
            result = new Bool(!dynamic_cast<const Bool&>(*temp));
            break;
        default:
            throw std::runtime_error("r u kidding me with a wrong unary op?");
    }
    delete temp;
    return result;
}
const Object* Interpreter::visitGroupingExpr(const Grouping *expr) 
{
    return evaluate(expr->expr);
}
const Object* Interpreter::visitLiteralExpr(const Literal *expr) 
{
    //默认拷贝。
    //所有的字面量都是临时常量
    return Object::clone(expr->value);
}
const Object*  Interpreter::visitVariableExpr(const Variable *expr) 
{
    return Object::clone(environment->get(expr->name));
}

void  Interpreter::visitExpressionStmt(const Expression *stmt) 
{
    evaluate(stmt->expr);
}
void  Interpreter::visitPrintStmt(const Print *stmt) 
{
   const Object* value = interprete(stmt->expr);
   std::cout<<Object::toString(value)<<endl;
}

void Interpreter::visitVarStmt(const Var *stmt)
{
    const Object* value = nullptr;
    if (stmt->initializer != nullptr) {
        value = evaluate(stmt->initializer);
    }
    this->environment->define(stmt->name->lexeme, value);
}
void Interpreter::visitBlockStmt(const Block *stmt)
{
    executeBlock(stmt->statements, new Environment(environment));
}

void Interpreter::visitIfStmt(const If *stmt)
{

    const Bool* value = dynamic_cast<const Bool*>(evaluate(stmt->condition));
    if (*value) {
        execute(stmt->thenBranch);
    } else {
        execute(stmt->elseBranch);
    } 
}

void Interpreter::printEnvironment()
{
    Environment *temp = this->environment;
    int scopeCount = 0;
    while (temp != nullptr) {
        cout<<"SCOPE:"<<scopeCount<<endl;
        temp->print();
        temp = temp->enclosing;
    }
}

void Interpreter::visitWhileStmt(const While *stmt) 
{
    while (*dynamic_cast<const Bool*>(evaluate(stmt->condition))) {
        execute(stmt->body);
    }
}

const Object* Interpreter::visitAssignExpr(const Assign *expr)
{
    const Object* value = evaluate(expr->value);
    environment->assign(expr->name, value);
    return Object::clone(value);
}

const Object* Interpreter::visitLogicalExpr(const Logical *expr)
{
    const Bool *left = dynamic_cast<const Bool*>(evaluate(expr->left));
    if (expr->op->type == AND) {
        if (!*left) return left;
    } else {
        if (*left) return left;
    }
    delete left;
    return evaluate(expr->right);
}

const Object* Interpreter::visitCallExpr(const Call *expr)
{
    const Object* callee = (evaluate(expr->callee));

    vector<const Object*> arguments;
    for (auto p : expr->arguments) {
        arguments.push_back(evaluate(p));
    }

    const Callable* function = dynamic_cast<const Callable*>(callee);
    if (function == nullptr) {
        throw RuntimeError(expr->paren, "can only call functions and classes");
    }
    if (arguments.size() != function->arity()) {
        ostringstream oss;
        oss<<"Expected "<<function->arity() + "arguments but got"<<
            arguments.size()<<".";
        throw RuntimeError(expr->paren, oss.str());
    }

    //return new Object((*callee)(arguments));
    return nullptr;
}

void Interpreter::checkStringOrNumber(const Token *op, const Object &l, const Object &r)
{
    if (typeid(l) == typeid(const Number) && typeid(r) == typeid(const Number)) return;
    if (typeid(l) == typeid(const String)) return;
    throw RuntimeError(op, "operands should be numbers or strings ");
}
void Interpreter::chechNumber(const Token *op, const Object &v)
{
    if (typeid(v) == typeid(const Number)) return;
    throw RuntimeError(op, "Operand must be a number");
}
void Interpreter::chechNumber(const Token *op, const Object &l, const  Object &r)
{
    if (typeid(l) == typeid(const Number) && typeid(r) == typeid(const Number)) return;
    throw RuntimeError(op, "operands should be numbers");
}
