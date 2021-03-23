/*================================================================
 *
 *
 *   FileName: Interpreter.h
 *   Author: DarrenHuang
 *   Create Time: 2020/06/20  16:37(Saturday)
 *   Description:
 *
 ================================================================*/
#ifndef _INTERPRETER_H_
#define _INTERPRETER_H_

#include <initializer_list>
#include <memory>
#include <vector>

#include "Environment.h"
#include "Stmt.h"
#include "Value.h"

namespace clox {
namespace runtime {

using std::vector;
using namespace compiling;

typedef std::shared_ptr<Stmt> StmtPtr;

// tree walker
class Interpreter : public Expr::Visitor, public Stmt::Visitor {
  using RETURN_TYPE = Expr::RETURN_TYPE;

public:
  Interpreter();
  void interprete(vector<StmtPtr> statements);

  RETURN_TYPE interprete(shared_ptr<Expr> expr);
  RETURN_TYPE evaluate(shared_ptr<Expr> expr);
  void execute(shared_ptr<Stmt> stmt);
  void executeBlock(vector<StmtPtr> stmts, shared_ptr<Environment> environment);

  void resolve(shared_ptr<Expr> expr, int depth) { locals[expr] = depth; }

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

  // aditional funcs for debug
  void printEnvironment();

public:
  shared_ptr<Environment> globals;
  shared_ptr<Environment> environment;

private:
  RETURN_TYPE lookUpVariable(shared_ptr<Token> token, shared_ptr<Expr> key);
  // check operant type
  static void checkStringOrNumber(shared_ptr<Token> op, RETURN_TYPE l,
                                  RETURN_TYPE r);
  static void chechNumber(shared_ptr<Token> op, RETURN_TYPE v);
  static void chechNumber(shared_ptr<Token> op, RETURN_TYPE l, RETURN_TYPE r);

private:
  std::map<shared_ptr<Expr>, int> locals;
};
} // namespace runtime
} // namespace clox

#endif
