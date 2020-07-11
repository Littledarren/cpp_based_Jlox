

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

Value Interpreter::interprete(Expr *expr)
{
    try {
        Value *temp = (Value*)evaluate(expr);
        //because i define deep copy 
        //so this is allowed;
        Value result(*temp);
        delete temp;
        //because defined deep copy so allowed
        return result;
    } catch (const char *str) {
        std::cout<<str<<std::endl;
    } catch (const RuntimeError &e) {
        runtimeError(e);
    }
    return Value();
}


void* Interpreter::evaluate(Expr *expr)
{
    return expr->accept(this);
}
void Interpreter::execute(Stmt * stmt)
{
    stmt->accept(this);
}
void Interpreter::executeBlock(vector<Stmt*> stmts, Environment *environment)
{
    Environment *previous = this->environment;
    try {
        this->environment = environment;
        for (Stmt *statement : stmts) {
            execute(statement);
        }
    } catch (const exception &e) {
        cout<<e.what()<<endl;
    }
    delete this->environment;
    this->environment = previous;
}
void* Interpreter::visitTernaryExpr(Ternary *expr) 
{
    return nullptr;
}
void* Interpreter::visitBinaryExpr(Binary *expr) 
{
    //this will creat a new ..
    Value *left = (Value*)evaluate(expr->left);
    Value *right = (Value*)evaluate(expr->right);
    Value *result = new Value;
    switch(expr->op->type)
    {
        case PLUS:
            checkStringOrNumber(PLUS, *left, *right);
            *result =  *left + *right;
            break;
        case MINUS:
            chechNumber(MINUS, *left, *right);
            *result = *left - *right;
            break;
        case STAR:
            chechNumber(MINUS, *left, *right);
            *result =  *left * *right;
            break;
        case SLASH:
            chechNumber(MINUS, *left, *right);
            *result = *left / *right;
            break;
        case COMMA:
            *result = *right;
            break;
        case GREATER:
            chechNumber(MINUS, *left, *right);
            *result = *left > *right;
            break;
        case GREATER_EQUAL:
            chechNumber(MINUS, *left, *right);
            *result = *left >= *right;
            break;
        case LESS:
            chechNumber(MINUS, *left, *right);
            *result = *left < *right;
            break;
        case LESS_EQUAL:
            chechNumber(MINUS, *left, *right);
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
    Value *temp = (Value*)evaluate(expr->right);
    Value *result = nullptr;
    switch(expr->op->type) {
        case MINUS:
            result = new Value(-*temp);
            break;
        case BANG:
            result = new Value(!*temp);
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
    return new Value(*(expr->type), expr->value);
}
void * Interpreter::visitVariableExpr(Variable *expr) 
{
    //Enexpr->name->lexeme
    return new Value(*environment->get(expr->name));
}

void * Interpreter::visitExpressionStmt(Expression *stmt) 
{
    evaluate(stmt->expr);
    // EYE 
    //may be i can delete stmt here.
    //after execute it delete it.
    return nullptr;
}
void * Interpreter::visitPrintStmt(Print *stmt) 
{
   Value value = interprete(stmt->expr);
   std::cout<<Value::toString(value)<<endl;
   return nullptr;
}

void* Interpreter::visitVarStmt(Var *stmt)
{
    Value * value = nullptr;
    if (stmt->initializer != nullptr) {
        value = (Value*)evaluate(stmt->initializer);
    }
    this->environment->define(stmt->name->lexeme, value);
    return nullptr;
}

void* Interpreter::visitAssignExpr(Assign *expr)
{
    Value *value = (Value*)evaluate(expr->value);
    environment->assign(expr->name, value);
    return new Value(*value);
}

void* Interpreter::visitBlockStmt(Block *stmt)
{
    executeBlock(stmt->statements, new Environment(environment));
    return nullptr;
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

void Interpreter::checkStringOrNumber(TokenType op, const Value &l, const Value &r)
{
    if (l.type == NUMBER && r.type == NUMBER) return;
    if (l.type == STRING && r.type == STRING) return;
    throw RuntimeError(op, "operands should be numbers");
}
void Interpreter::chechNumber(TokenType op, const Value &v)
{
    if (v.type != NUMBER) throw RuntimeError(op, "Operand must be a number");
}
void Interpreter::chechNumber(TokenType op, const Value &l, const  Value &r)
{
    if (l.type != NUMBER || r.type != NUMBER) throw RuntimeError(op, "operands should be numbers");
}
