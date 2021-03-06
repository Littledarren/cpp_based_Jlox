/*================================================================
*
*
*   FileName: Resolver.cpp
*   Author: DarrenHuang
*   Create Time: 2021/03/17  10:54(星期三) *   Description:
*
================================================================*/
#include "Resolver.h"

namespace clox {
namespace compiling {

void Resolver::resolve(const vector<shared_ptr<Stmt>> &statements) {
  for (auto &p : statements) {
    resolve<Stmt>(p);
  }
}

// expr
Resolver::RETURN_TYPE Resolver::visit(shared_ptr<Assign> expr) {
  resolve(expr->value);
  resolveLocal(expr, expr->name);
  return nullptr;
}
Resolver::RETURN_TYPE Resolver::visit(shared_ptr<Binary> expr) {
  resolve(expr->left);
  resolve(expr->right);
  return nullptr;
}
Resolver::RETURN_TYPE Resolver::visit(shared_ptr<Grouping> expr) {
  resolve(expr->expr);
  return nullptr;
}
Resolver::RETURN_TYPE Resolver::visit(shared_ptr<Literal> expr) {
  return nullptr;
}
Resolver::RETURN_TYPE Resolver::visit(shared_ptr<Unary> expr) {
  resolve(expr->right);
  return nullptr;
}
Resolver::RETURN_TYPE Resolver::visit(shared_ptr<Ternary> expr) {
  resolve(expr->condition);
  resolve(expr->if_yes);
  resolve(expr->if_no);
  return nullptr;
}
Resolver::RETURN_TYPE Resolver::visit(shared_ptr<Logical> expr) {
  resolve(expr->left);
  resolve(expr->right);
  return nullptr;
}
Resolver::RETURN_TYPE Resolver::visit(shared_ptr<Variable> expr) {
  if (!scopes.empty()
      //当前域声明但未定义
      // java的get如果不存在返回null,c++必须先判断一下，如果不想出异常的话
      && scopes.back().find(expr->name->lexeme) != scopes.back().end() &&
      !scopes.back()[expr->name->lexeme]) {
    error::error(*expr->name,
                 "Can't read local variable in its own initializer");
  }

  resolveLocal(expr, expr->name);
  return nullptr;
}
Resolver::RETURN_TYPE Resolver::visit(shared_ptr<Call> expr) {
  resolve(expr->callee);

  for (auto &p : expr->arguments) {
    resolve(p);
  }
  return nullptr;
}
Resolver::RETURN_TYPE Resolver::visit(shared_ptr<Lambda> expr) {
  //感觉这里应该让expr的lambda优先级更高。。。
  //函数是有名字的lambda
  // Function = name + lambda
  // LoxFunction = Function + ....(named lambda)
  //              ^^^^^^^^-> null+lambda (anonymous lambda)
  resolveLambda(expr, FunctionType::FUNCTION);
  return nullptr;
}

Resolver::RETURN_TYPE Resolver::visit(shared_ptr<Get> expr) {
  resolve(expr->expr);
  return nullptr;
}
Resolver::RETURN_TYPE Resolver::visit(shared_ptr<Set> expr) {
  resolve(expr->value);
  resolve(expr->obj);
  return nullptr;
}
Resolver::RETURN_TYPE Resolver::visit(shared_ptr<This> expr) {
  if (currentClass == ClassType::NONE) {
    error::error(*expr->keyword, "Can't use 'this' outside of a class");
  }
  resolveLocal(expr, expr->keyword);
  return nullptr;
}
Resolver::RETURN_TYPE Resolver::visit(shared_ptr<Super> expr) {
  switch (currentClass) {
  case ClassType::NONE:
    error::error(*expr->keyword, "Can't use 'super' outside of a class");
    break;
  case ClassType::CLASS:
    error::error(*expr->keyword,
                 "Can't use 'super' in a class with no super class");
    break;
  default:
    break;
  }
  resolveLocal(expr, expr->keyword);
  return nullptr;
}
// Stmt
void Resolver::visit(shared_ptr<Expression> stmt) { resolve(stmt->expr); }
void Resolver::visit(shared_ptr<Print> stmt) { resolve(stmt->expr); }
void Resolver::visit(shared_ptr<Var> stmt) {
  declare(stmt->name);
  if (stmt->initializer) {
    resolve(stmt->initializer);
  }
  define(stmt->name);
}
void Resolver::visit(shared_ptr<Block> stmt) {
  beginScope();
  resolve(stmt->statements);
  endScope();
}
void Resolver::visit(shared_ptr<If> stmt) {
  resolve(stmt->condition);
  resolve(stmt->thenBranch);
  if (stmt->elseBranch)
    resolve(stmt->elseBranch);
}
void Resolver::visit(shared_ptr<While> stmt) {
  resolve(stmt->condition);
  resolve(stmt->body);
}
void Resolver::visit(shared_ptr<Function> func) {
  declare(func->name);
  define(func->name);
  resolveLambda(func->lambda, FunctionType::FUNCTION);
}
void Resolver::visit(shared_ptr<Return> stmt) {
  if (currentFunction == FunctionType::NONE) {
    error::error(*stmt->name, "Can't return from top-level code");
  }
  if (stmt->value) {
    if (currentFunction == FunctionType::INITIALIZER) {
      error::error(*stmt->name, "Can't return a value from an initializer");
    }
    resolve(stmt->value);
  }
}

void Resolver::visit(shared_ptr<Class> stmt) {
  ClassType enclosingClass = currentClass;
  currentClass = ClassType::CLASS;
  declare(stmt->name);
  define(stmt->name);
  if (stmt->super_class &&
      stmt->super_class->name->lexeme == stmt->name->lexeme) {
    error::error(*stmt->super_class->name, "A class can't inherit from itself");
  }
  if (stmt->super_class) {
    currentClass = ClassType::SUBCLASS;
    resolve(stmt->super_class);
  }

  if (stmt->super_class) {
    beginScope();
    scopes.back()["super"] = true;
  }

  beginScope();
  //解析类的方法
  scopes.back()["this"] = true;
  for (auto &method : stmt->methods) {
    //在语法解析时候确定type
    resolveLambda(method->lambda, method->type);
  }
  endScope();
  if (stmt->super_class) {
    endScope();
  }
  currentClass = enclosingClass;
}
} // namespace compiling
} // namespace clox
