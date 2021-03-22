/*================================================================
*
*
*   FileName: Resolver.h
*   Author: DarrenHuang
*   Create Time: 2021/03/17  10:50(星期三)
*   Description:
*
================================================================*/

#ifndef RESOLVER_H
#define RESOLVER_H

#include <map>
#include <vector>
using std::map;
using std::vector;

// 子类必须包含父类的头文件
#include "Stmt.h"
#include "Value.h"

//或许可以弄到cpp中，不过应该是强依赖关系？
#include "Interpreter.h"
#include "main.h"

enum class ClassType { NONE, CLASS };

// 本质上语法树已经给出来了，很多工作都可以基于AST做，所以Treewalker也好，
// Resolver也好，都是一样的

class Resolver : public Expr::Visitor, public Stmt::Visitor {
  using RETURN_TYPE = Expr::RETURN_TYPE;

public:
  Resolver(Interpreter &interpreter) : interpreter(interpreter) {}
  virtual ~Resolver() = default;
  void resolve(const vector<shared_ptr<Stmt>> &statements);

  // expr
  virtual RETURN_TYPE visit(shared_ptr<Assign> expr) override;
  virtual RETURN_TYPE visit(shared_ptr<Binary> expr) override;
  virtual RETURN_TYPE visit(shared_ptr<Grouping> expr) override;
  virtual RETURN_TYPE visit(shared_ptr<Literal> expr) override;
  virtual RETURN_TYPE visit(shared_ptr<Unary> expr) override;
  virtual RETURN_TYPE visit(shared_ptr<Ternary> expr) override;
  virtual RETURN_TYPE visit(shared_ptr<Logical> expr) override;
  virtual RETURN_TYPE visit(shared_ptr<Variable> expr) override;
  virtual RETURN_TYPE visit(shared_ptr<Call> expr) override;
  virtual RETURN_TYPE visit(shared_ptr<Lambda> expr) override;
  virtual RETURN_TYPE visit(shared_ptr<Get> expr) override;
  virtual RETURN_TYPE visit(shared_ptr<Set> expr) override;
  virtual RETURN_TYPE visit(shared_ptr<This> expr) override;
  // Stmt
  virtual void visit(shared_ptr<Expression> stmt) override;
  virtual void visit(shared_ptr<Print> stmt) override;
  virtual void visit(shared_ptr<Var> stmt) override;
  virtual void visit(shared_ptr<Block> stmt) override;
  virtual void visit(shared_ptr<If> stmt) override;
  virtual void visit(shared_ptr<Function> func) override;
  virtual void visit(shared_ptr<While> stmt) override;
  virtual void visit(shared_ptr<Return> stmt) override;
  virtual void visit(shared_ptr<Class> stmt) override;

private:
  // void resolve(const shared_ptr<Stmt> &stmt);
  template <typename T> void resolve(const shared_ptr<T> &t) {
    if (t)
      t->accept(*this);
    else
      throw std::runtime_error("Resolver");
  }

  void declare(const shared_ptr<Token> &name) {
    if (scopes.empty())
      return;

    auto &scope = scopes.back();
    if (scope.find(name->lexeme) != scope.end()) {
      ::error(*name, "already variable with this name in this scope");
    }
    scope[name->lexeme] = false;
  }

  void define(const shared_ptr<Token> &name) {
    if (scopes.empty())
      return;
    auto &scope = scopes.back();
    scope[name->lexeme] = true;
  }

  void resolveLocal(shared_ptr<Expr> expr, const shared_ptr<Token> &name) {
    // 主要是看变量究竟在第几层，然后就固定不动了，不会每次都找
    // 即变量的绑定是在运行之前
    for (int i = scopes.size() - 1; i >= 0; --i) {
      if (scopes.at(i).count(name->lexeme)) {
        interpreter.resolve(expr, scopes.size() - 1 - i);
        return;
      }
    }
    //::error(*name, "can not find in this scope");
  }

  void resolveLambda(shared_ptr<Lambda> func, FunctionType type) {
    FunctionType enclosingFunction = currentFunction;
    currentFunction = type;

    beginScope();
    for (auto &param : func->params) {
      declare(param);
      define(param);
    }
    resolve(func->body);
    endScope();
    currentFunction = enclosingFunction;
  }

  void beginScope() { scopes.push_back(map<string, bool>()); }
  void endScope() { scopes.pop_back(); }

private:
  //类内初始化
  FunctionType currentFunction = FunctionType::NONE;
  ClassType currentClass = ClassType::NONE;
  vector<map<string, bool>> scopes;
  Interpreter &interpreter;
};

#endif /* RESOLVER_H */
