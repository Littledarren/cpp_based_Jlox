#include <exception>
#include "../includes/Interpreter.h"
#include "../includes/main.h"




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

CObjectPtr Interpreter::interprete(CExprPtr expr)
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


CObjectPtr Interpreter::evaluate(CExprPtr expr)
{
    if (expr)
        return expr->accept(this);
    else 
        return nullptr;
}
void Interpreter::execute(CStmtPtr stmt)
{
    if (stmt)
        stmt->accept(this);
}
void Interpreter::executeBlock(vector<shared_ptr<Stmt>> stmts, shared_ptr<Environment> environment)
{
    shared_ptr<Environment> previous = this->environment;
    try {
        this->environment = (environment);
        for (auto ptr : stmts) {
            execute(ptr);
        }
    } catch (const std::exception &e) {
        cout<<e.what()<<endl;
    }
    this->environment = previous;
}
CObjectPtr Interpreter::visitTernaryExpr(const Ternary *expr) 
{
    return nullptr;
}
CObjectPtr Interpreter::visitBinaryExpr(const Binary *expr) 
{
    //this will creat a new ..
    CObjectPtr left = evaluate(expr->left);
    CObjectPtr right = evaluate(expr->right);


    bool is_string = !!std::dynamic_pointer_cast<const String>(left);

    CObjectPtr result;

    switch(expr->op->type)
    {
        case PLUS:
            checkStringOrNumber(expr->op, left, right);
            if (is_string) {
                if (auto temp_number = std::dynamic_pointer_cast<const Number>(right)) 
                    result = std::make_shared<String>(*std::dynamic_pointer_cast<const String>(left) + *temp_number);
                else if (auto temp_bool = std::dynamic_pointer_cast<const Bool>(right))
                    result = std::make_shared<String>(*std::dynamic_pointer_cast<const String>(left) + *temp_bool);
                else
                    result = std::make_shared<String>((string)*std::dynamic_pointer_cast<const String>(left) + (string)*std::dynamic_pointer_cast<const String>(right));
            } else
                result = std::make_shared<Number>(*std::dynamic_pointer_cast<const Number>(left) + *std::dynamic_pointer_cast<const Number>(right));
            break;
        case MINUS:
            chechNumber(expr->op, left, right);
            result = std::make_shared<Number>(*std::dynamic_pointer_cast<const Number>(left) - *std::dynamic_pointer_cast<const Number>(right));
            break;
        case STAR:
            chechNumber(expr->op, left, right);
            result = std::make_shared<Number>(*std::dynamic_pointer_cast<const Number>(left) * *std::dynamic_pointer_cast<const Number>(right));
            break;
        case SLASH:
            chechNumber(expr->op, left, right);
            result = std::make_shared<Number>(*std::dynamic_pointer_cast<const Number>(left) / *std::dynamic_pointer_cast<const Number>(right));
            break;
        case COMMA:
            return right;
            break;
        case GREATER:
            chechNumber(expr->op, left, right);
            result = std::make_shared<Bool>(*std::dynamic_pointer_cast<const Number>(left) > *std::dynamic_pointer_cast<const Number>(right));
            break;
        case GREATER_EQUAL:
            chechNumber(expr->op, left, right);
            result = std::make_shared<Bool>(*std::dynamic_pointer_cast<const Number>(left) >= *std::dynamic_pointer_cast<const Number>(right));
            break;
        case LESS:
            chechNumber(expr->op, left, right);
            result = std::make_shared<Bool>(*std::dynamic_pointer_cast<const Number>(left) < *std::dynamic_pointer_cast<const Number>(right));
            break;
        case LESS_EQUAL:
            chechNumber(expr->op, left, right);
            result = std::make_shared<Bool>(*std::dynamic_pointer_cast<const Number>(left) <= *std::dynamic_pointer_cast<const Number>(right));
            break;
        case EQUAL_EQUAL:
            result = std::make_shared<Bool>(left==right);
            break;
        case BANG_EQUAL:
            result = std::make_shared<Bool>(!(left==right));
            break;
        default:
            throw string("ERROR UNKONE OP") + expr->op->lexeme;
    }
    return result;
}
CObjectPtr Interpreter::visitUnaryExpr(const Unary *expr) 
{
    CObjectPtr temp = evaluate(expr->right);
    CObjectPtr result = nullptr;
    switch(expr->op->type) {
        case MINUS:
            result = std::make_shared<Number>(-*std::dynamic_pointer_cast<const Number>(temp));
            break;
        case BANG:
            result = std::make_shared<Bool>(!*std::dynamic_pointer_cast<const Bool>(temp));
            break;
        default:
            throw std::runtime_error("r u kidding me with a wrong unary op?");
    }
    return result;
}
CObjectPtr Interpreter::visitGroupingExpr(const Grouping *expr) 
{
    return evaluate(expr->expr);
}
CObjectPtr Interpreter::visitLiteralExpr(const Literal *expr) 
{
    //默认拷贝。
    //所有的字面量都是临时常量
    return expr->value;
    //return Object::clone(expr->value);
}
CObjectPtr  Interpreter::visitVariableExpr(const Variable *expr) 
{
    return environment->get(expr->name);
}

void  Interpreter::visitExpressionStmt(const Expression *stmt) 
{
    evaluate(stmt->expr);
}
void  Interpreter::visitPrintStmt(const Print *stmt) 
{
   CObjectPtr value = interprete(stmt->expr);
   std::cout<<(value)->toString()<<endl;
}

void Interpreter::visitVarStmt(const Var *stmt)
{
    CObjectPtr value = nullptr;
    if (stmt->initializer != nullptr) {
        value = evaluate(stmt->initializer);
    }
    this->environment->define(stmt->name->lexeme, value);
}
void Interpreter::visitBlockStmt(const Block *stmt)
{
    executeBlock(stmt->statements, std::make_shared<Environment>(environment));
}

void Interpreter::visitIfStmt(const If *stmt)
{
    shared_ptr<const Bool> value = std::dynamic_pointer_cast<const Bool>(evaluate(stmt->condition));

    if (*value) {
        execute(stmt->thenBranch);
    } else {
        execute(stmt->elseBranch);
    } 
}

void Interpreter::printEnvironment()
{
    shared_ptr<Environment> temp = this->environment;
    int scopeCount = 0;
    while (temp) {
        cout<<"SCOPE:"<<scopeCount<<endl;
        temp->print();
        temp = temp->enclosing;
    }
}

void Interpreter::visitWhileStmt(const While *stmt) 
{
    while (*std::dynamic_pointer_cast<const Bool>(evaluate(stmt->condition))) {
        execute(stmt->body);
    }
}

CObjectPtr Interpreter::visitAssignExpr(const Assign *expr)
{
    CObjectPtr value = evaluate(expr->value);
    environment->assign(expr->name, value);
    //may be clone???
    return value;
}

CObjectPtr Interpreter::visitLogicalExpr(const Logical *expr)
{
    shared_ptr<const Bool> left = std::dynamic_pointer_cast<const Bool>(evaluate(expr->left));
    if (expr->op->type == AND) {
        if (!*left) return left;
    } else {
        if (*left) return left;
    }
    return evaluate(expr->right);
}

CObjectPtr Interpreter::visitCallExpr(const Call *expr)
{
    CObjectPtr callee = (evaluate(expr->callee));

    vector<CObjectPtr> arguments;
    for (auto p : expr->arguments) {
        arguments.push_back(evaluate(p));
    }

    shared_ptr<const Callable> function = std::dynamic_pointer_cast<const Callable>(callee);
    if (!function) {
        throw RuntimeError(expr->paren, "can only call functions and classes");
    }
    if (arguments.size() != function->arity()) {
        ostringstream oss;
        oss<<"Expected "<<function->arity() << "arguments but got"<<
            arguments.size()<<".";
        throw RuntimeError(expr->paren, oss.str());
    }

    //return new Object((*callee)(arguments));
    return nullptr;
}

void Interpreter::checkStringOrNumber(shared_ptr<const Token>op, CObjectPtr l, CObjectPtr r)
{
    if (std::dynamic_pointer_cast<const Number>(l) && std::dynamic_pointer_cast<const Number>(r)) return;
    if (std::dynamic_pointer_cast<const String>(l)) return;
    throw RuntimeError(op, "operands should be numbers or strings ");
}
void Interpreter::chechNumber(shared_ptr<const Token>op, CObjectPtr v)
{
    if (std::dynamic_pointer_cast<const Number>(v)) return;
    throw RuntimeError(op, "Operand must be a number");
}
void Interpreter::chechNumber(shared_ptr<const Token>op, CObjectPtr l, CObjectPtr r)
{
    if (std::dynamic_pointer_cast<const Number>(l) && std::dynamic_pointer_cast<const Number>(r)) return;
    throw RuntimeError(op, "operands should be numbers");
}
