/*================================================================
*    
*   
*   FileName: Resolver.cpp
*   Author: DarrenHuang
*   Create Time: 2021/03/17  10:54(星期三) *   Description:
*
================================================================*/


#include "Resolver.h"

void Resolver::resolve(const vector<shared_ptr<Stmt>> &statements)
{
    for (auto& p : statements) {
        resolve<Stmt>(p);
    }
}


//expr
RETURN_TYPE Resolver::visit(const Assign &expr) 
{
    resolve(expr.value);
    resolveLocal(expr, expr.name);
    return nullptr;
}
RETURN_TYPE Resolver::visit(const Binary &expr) 
{
    resolve(expr.left);
    resolve(expr.right);
    return nullptr;
}
RETURN_TYPE Resolver::visit(const Grouping &expr) 
{
    resolve(expr.expr);
    return nullptr;
}
RETURN_TYPE Resolver::visit(const Literal &expr) 
{
    return nullptr;
}
RETURN_TYPE Resolver::visit(const Unary &expr) 
{
    resolve(expr.right);
    return nullptr;
}
RETURN_TYPE Resolver::visit(const Ternary &expr) 
{
    resolve(expr.condition);
    resolve(expr.if_yes);
    resolve(expr.if_no);
    return nullptr;
}
RETURN_TYPE Resolver::visit(const Logical &expr) 
{
    resolve(expr.left);
    resolve(expr.right);
    return nullptr;
}
RETURN_TYPE Resolver::visit(const Variable &expr) 
{
    if (!scopes.empty()
            //当前域声明但未定义
            //java的get如果不存在返回null,c++必须先判断一下，如果不想出异常的话
            && scopes.back().find(expr.name->lexeme) != scopes.back().end()
            && !scopes.back()[expr.name->lexeme])  {
       ::error(*expr.name, "Can't read local variable in its own initializer");
    }

    resolveLocal(expr, expr.name);
    return nullptr;
}
RETURN_TYPE Resolver::visit(const Call &expr) 
{
    resolve(expr.callee);

    for (auto& p : expr.arguments) {
        resolve(p);
    }
    return nullptr;
}
RETURN_TYPE Resolver::visit(const Lambda &expr) 
{
    //感觉这里应该让expr的lambda优先级更高。。。
    //函数是有名字的lambda
    //Function = name + lambda
    //LoxFunction = Function + ....(named lambda)
    //              ^^^^^^^^-> null+lambda (anonymous lambda)
    resolveLambda(expr, FunctionType::FUNCTION);
    return nullptr;
}

//Stmt
void Resolver::visit(const Expression &stmt) 
{
    resolve(stmt.expr);
}
void Resolver::visit(const Print &stmt) 
{
    resolve(stmt.expr);
}
void Resolver::visit(const Var &stmt) 
{
    declare(stmt.name);
    if (stmt.initializer) {
        resolve(stmt.initializer);
    }
    define(stmt.name);
}
void Resolver::visit(const Block &stmt) 
{
    beginScope();
    resolve(stmt.statements);
    endScope();
}
void Resolver::visit(const If &stmt) 
{
    resolve(stmt.condition);
    resolve(stmt.thenBranch);
    if (stmt.elseBranch) resolve(stmt.elseBranch);
}
void Resolver::visit(const While &stmt) 
{
    resolve(stmt.condition);
    resolve(stmt.body);
}
void Resolver::visit(const Function &func) 
{
    declare(func.name);
    define(func.name);
    resolveLambda(func.lambda, FunctionType::FUNCTION);
}
void Resolver::visit(const Return&stmt) 
{
    if (currentFunction == FunctionType::NONE) {
        ::error(*stmt.name, "Can't return from top-level code");
    }
    if (stmt.value) {
        resolve(stmt.value);
    }
}

