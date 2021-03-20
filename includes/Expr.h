/*================================================================
 *
 *
 *   FileName: Expr.h
 *   Author: DarrenHuang
 *   Create Time: 2020/06/17  09:49(Wednesday)
 *   Description:
 *      Abstract Syntax Tree of Expr
 *
 表达式语法树，大部分的表达式由其他表达式和单词构成，除了lambda表达式有语句，literal只有值
 *
 ================================================================*/
#ifndef _EXPR_H_
#define _EXPR_H_

#include <iostream>
#include <memory>
#include <vector>

#include "Token.h"

using std::string;
using std::vector;

//用于继承，
#define ENABLED(Base, Derived)                                                 \
public                                                                         \
  Base, public std::enable_shared_from_this<Derived>

//赋值表达式
// Assign : IDENTIFIER '=' expr
struct Assign;
//二元表达式
// Binary : expr (+|-|*|/|,|>|<|>=|<=|==|!=) expr
struct Binary;
//单目运算符
// Unary : (-|!|+) expr
struct Unary;
// 括号(用于改变优先级)
// Grouping : '('expr')'
struct Grouping;
// 字面量
// Literal : true | false | STRING | NUMBER | Nil
struct Literal;
// 三元运算
// Ternary : expr ? expr : expr
struct Ternary;
// 变量
// Variable : IDENTIFIER
struct Variable;
// 逻辑运算
// Logical : expr (and | or) expr
struct Logical;
//调用 呃，这里的正则不会写
// Call : IDENTIFIER'(' expr? (',' expr)*')''
struct Call;
struct Lambda;
struct Get;
struct Set;
struct This;

struct Expr {
  using RETURN_TYPE = shared_ptr<Object>;
  struct Visitor {
    virtual RETURN_TYPE visit(shared_ptr<Assign> expr) = 0;
    virtual RETURN_TYPE visit(shared_ptr<Binary> expr) = 0;
    virtual RETURN_TYPE visit(shared_ptr<Grouping> expr) = 0;
    virtual RETURN_TYPE visit(shared_ptr<Literal> expr) = 0;
    virtual RETURN_TYPE visit(shared_ptr<Unary> expr) = 0;
    virtual RETURN_TYPE visit(shared_ptr<Ternary> expr) = 0;
    virtual RETURN_TYPE visit(shared_ptr<Logical> expr) = 0;
    virtual RETURN_TYPE visit(shared_ptr<Variable> expr) = 0;
    virtual RETURN_TYPE visit(shared_ptr<Call> expr) = 0;
    virtual RETURN_TYPE visit(shared_ptr<Lambda> expr) = 0;
    virtual RETURN_TYPE visit(shared_ptr<Get> expr) = 0;
    virtual RETURN_TYPE visit(shared_ptr<Set> expr) = 0;
    virtual RETURN_TYPE visit(shared_ptr<This> expr) = 0;
  };

  virtual RETURN_TYPE accept(Visitor &visitor) = 0;

#define EXPR_VISITABLE()                                                       \
  RETURN_TYPE accept(Visitor &visitor) override {                              \
    return visitor.visit(this->shared_from_this());                            \
  }
};

struct Assign : ENABLED(Expr, Assign) {
  Assign(shared_ptr<Token> name, shared_ptr<Expr> value)
      : name(name), value(value) {}

  EXPR_VISITABLE()

  shared_ptr<Token> name;
  shared_ptr<Expr> value;
};

struct Binary : ENABLED(Expr, Binary) {
  Binary(shared_ptr<Expr> left, shared_ptr<Token> op, shared_ptr<Expr> right)
      : left(left), op(op), right(right) {}
  EXPR_VISITABLE();
  shared_ptr<Expr> left;
  shared_ptr<Token> op;
  shared_ptr<Expr> right;
};

struct Ternary : ENABLED(Expr, Ternary) {
  Ternary(shared_ptr<Expr> condition, shared_ptr<Expr> if_yes,
          shared_ptr<Expr> if_no)
      : condition(condition), if_yes(if_yes), if_no(if_no) {}
  EXPR_VISITABLE();

  shared_ptr<Expr> condition;
  shared_ptr<Expr> if_yes;
  shared_ptr<Expr> if_no;
};

struct Grouping : ENABLED(Expr, Grouping) {
  Grouping(shared_ptr<Expr> expr) : expr(expr) {}
  EXPR_VISITABLE();
  shared_ptr<Expr> expr;
};

struct Literal : ENABLED(Expr, Literal) {
  explicit Literal(RETURN_TYPE value = nullptr) : value(value) {}

  string getStr() {
    if (!value)
      return "NIL";
    return value->toString();
  }

  EXPR_VISITABLE();

  // string, numbers, bool, Nil
  RETURN_TYPE value;
};

struct Unary : ENABLED(Expr, Unary) {
  Unary(shared_ptr<Token> op, shared_ptr<Expr> right) : op(op), right(right) {}
  EXPR_VISITABLE();
  shared_ptr<Token> op;
  shared_ptr<Expr> right;
};
struct Variable : ENABLED(Expr, Variable) {
  Variable(shared_ptr<Token> name) : name(name) {}
  EXPR_VISITABLE();
  shared_ptr<Token> name;
};

struct Logical : ENABLED(Expr, Logical) {
  Logical(shared_ptr<Expr> left, shared_ptr<Token> op, shared_ptr<Expr> right)
      : left(left), op(op), right(right) {}

  EXPR_VISITABLE();
  shared_ptr<Expr> left;
  shared_ptr<Token> op;
  shared_ptr<Expr> right;
};

struct Call : ENABLED(Expr, Call) {
  Call(shared_ptr<Expr> callee, shared_ptr<Token> paren,
       vector<shared_ptr<Expr>> &arguments)
      : callee(callee), paren(paren), arguments(arguments) {}
  EXPR_VISITABLE();
  shared_ptr<Expr> callee;
  shared_ptr<Token> paren;
  vector<shared_ptr<Expr>> arguments;
};
struct Stmt;
struct Lambda : ENABLED(Expr, Lambda) {
  Lambda(vector<shared_ptr<Token>> params, vector<shared_ptr<Stmt>> body)
      : params(params), body(body) {}

  EXPR_VISITABLE();
  vector<shared_ptr<Token>> params;
  vector<shared_ptr<Stmt>> body;
};

struct Get : ENABLED(Expr, Get) {
  Get(shared_ptr<Expr> expr, shared_ptr<Token> name) : expr(expr), name(name) {}

  EXPR_VISITABLE();
  shared_ptr<Expr> expr;
  shared_ptr<Token> name;
};

struct Set : ENABLED(Expr, Set) {
  Set(shared_ptr<Expr> obj, shared_ptr<Token> token, shared_ptr<Expr> value)
      : obj(obj), token(token), value(value) {}

  EXPR_VISITABLE();
  shared_ptr<Expr> obj;
  shared_ptr<Token> token;
  shared_ptr<Expr> value;
};
struct This : ENABLED(Expr, This) {
  This(shared_ptr<Token> keyword) : keyword(keyword) {}

  EXPR_VISITABLE();
  shared_ptr<Token> keyword;
};

#endif
