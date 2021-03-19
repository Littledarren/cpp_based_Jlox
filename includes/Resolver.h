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

enum class FunctionType { NONE, FUNCTION, METHOD, INITIALIZER };
enum class ClassType { NONE, CLASS};

// 本质上语法树已经给出来了，很多工作都可以基于AST做，所以Treewalker也好，
// Resolver也好，都是一样的

class Resolver : public Expr::Visitor, public Stmt::Visitor {
public:
  Resolver(Interpreter &interpreter) : interpreter(interpreter) {}
  virtual ~Resolver() = default;
  void resolve(const vector<shared_ptr<Stmt>> &statements);

  // expr
  RETURN_TYPE visit(const Assign &expr) override;
  RETURN_TYPE visit(const Binary &expr) override;
  RETURN_TYPE visit(const Grouping &expr) override;
  RETURN_TYPE visit(const Literal &expr) override;
  RETURN_TYPE visit(const Unary &expr) override;
  RETURN_TYPE visit(const Ternary &expr) override;
  RETURN_TYPE visit(const Logical &expr) override;
  RETURN_TYPE visit(const Variable &expr) override;
  RETURN_TYPE visit(const Call &expr) override;
  RETURN_TYPE visit(const Lambda &expr) override;
  RETURN_TYPE visit(const Get &expr) override;
  RETURN_TYPE visit(const Set &expr) override;
  RETURN_TYPE visit(const This &expr) override;
  // Stmt
  void visit(const Expression &stmt) override;
  void visit(const Print &stmt) override;
  void visit(const Var &stmt) override;
  void visit(const Block &stmt) override;
  void visit(const If &stmt) override;
  void visit(const While &stmt) override;
  void visit(const Function &func) override;
  void visit(const Return &stmt) override;
  void visit(const Class &stmt) override;

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

  void resolveLocal(const Expr &expr, const shared_ptr<Token> &name) {
    // 主要是看变量究竟在第几层，然后就固定不动了，不会每次都找
    // 即变量的绑定是在运行之前
    for (int i = scopes.size() - 1; i >= 0; --i) {
      if (scopes.at(i).count(name->lexeme)) {
        interpreter.resolve(expr, scopes.size() - 1 - i);
        return;
      }
    }
  }

  void resolveLambda(const Lambda &func, FunctionType type) {
    FunctionType enclosingFunction = currentFunction;
    currentFunction = type;
    beginScope();
    for (auto &param : func.params) {
      declare(param);
      define(param);
    }
    resolve(func.body);
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
