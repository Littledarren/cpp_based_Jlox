/*================================================================
 *
 *
 *   FileName: Stmt.h
 *   Author: DarrenHuang
 *   Create Time: 2020/07/04  09:30(星期六)
 *   Description:
 *   语法树，语句，通常由表达式和其他语句构成，没有返回值
 *
 ================================================================*/
#ifndef _STMT_H_
#define _STMT_H_

#include <vector>
using std::vector;

#include "Expr.h"

enum class FunctionType { NONE, FUNCTION, METHOD, INITIALIZER, STATIC_METHOD };

struct Expression;
struct Print;
//变量声明
struct Var;
struct Block;
struct If;
struct While;
//函数声明
struct Function;
struct Return;
//类声明
struct Class;

//注意，声明是语句而不是表达式，而调用是表达式而不是语句
//此外语法树是编译时的中间产物，虽然treewalker的实现方法直接在语法树上执行，但是
//如果不执行的话，完全是静态的，是编译时而非运行时，所以这就是Resolver的作用
//很多编译时的动作可以在语法树上进行
//
//心中有棵树，不是B树，是AST树

struct Stmt {
  struct Visitor {
    virtual void visit(shared_ptr<Expression> stmt) = 0;
    virtual void visit(shared_ptr<Print> stmt) = 0;
    virtual void visit(shared_ptr<Var> stmt) = 0;
    virtual void visit(shared_ptr<Block> stmt) = 0;
    virtual void visit(shared_ptr<If> stmt) = 0;
    virtual void visit(shared_ptr<Function> func) = 0;
    virtual void visit(shared_ptr<While> stmt) = 0;
    virtual void visit(shared_ptr<Return> stmt) = 0;
    virtual void visit(shared_ptr<Class> stmt) = 0;
  };

  virtual void accept(Visitor &visitor) = 0;

#define STMT_VISITABLE()                                                       \
  virtual void accept(Visitor &visitor) override {                             \
    visitor.visit(this->shared_from_this());                                   \
  }
};

struct Var : ENABLED(Stmt, Var) {
  Var(shared_ptr<Token> name, shared_ptr<Expr> initializer)
      : name(name), initializer(initializer) {}
  STMT_VISITABLE();
  shared_ptr<Token> name;
  shared_ptr<Expr> initializer;
};

struct Expression : ENABLED(Stmt, Expression) {
  Expression(shared_ptr<Expr> expr) : expr(expr) {}

  STMT_VISITABLE();

  shared_ptr<Expr> expr;
};

struct Print : ENABLED(Stmt, Print) {
  Print(shared_ptr<Expr> expr) : expr(expr) {}

  STMT_VISITABLE();
  shared_ptr<Expr> expr;
};

struct Block : ENABLED(Stmt, Block) {
  Block(const vector<shared_ptr<Stmt>> &statements) : statements(statements) {}
  STMT_VISITABLE();

  vector<shared_ptr<Stmt>> statements;
};

struct If : ENABLED(Stmt, If) {
  If(shared_ptr<Expr> condition, shared_ptr<Stmt> thenBranch,
     shared_ptr<Stmt> elseBranch)
      : condition(condition), thenBranch(thenBranch), elseBranch(elseBranch) {}
  STMT_VISITABLE();
  shared_ptr<Expr> condition;
  shared_ptr<Stmt> thenBranch;
  shared_ptr<Stmt> elseBranch;
};

struct While : ENABLED(Stmt, While) {
  While(shared_ptr<Expr> condition, shared_ptr<Stmt> body)
      : condition(condition), body(body) {}
  STMT_VISITABLE();

  shared_ptr<Expr> condition;
  shared_ptr<Stmt> body;
};
struct Function : ENABLED(Stmt, Function) {
  Function(shared_ptr<Token> name, shared_ptr<Lambda> lambda, FunctionType type)
      : name(name), lambda(lambda), type(type) {}

  STMT_VISITABLE();
  shared_ptr<Token> name;
  shared_ptr<Lambda> lambda;
  FunctionType type;
};

struct Return : ENABLED(Stmt, Return) {
  Return(shared_ptr<Token> name, shared_ptr<Expr> value)
      : name(name), value(value) {}

  STMT_VISITABLE();
  shared_ptr<Token> name;
  shared_ptr<Expr> value;
};

struct Class : ENABLED(Stmt, Class) {
  Class(shared_ptr<Token> name, const vector<shared_ptr<Function>> &methods)
      : name(name), methods(methods) {}

  STMT_VISITABLE();
  shared_ptr<Token> name;
  vector<shared_ptr<Function>> methods;
};

#endif
