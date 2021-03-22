/*================================================================
*
*
*   FileName: RPN_Printer.h
*   Author: DarrenHuang
*   Create Time: 2021/03/22  13:23(星期一)
*   Description:
*
================================================================*/

#ifndef RPN_PRINTER_H
#define RPN_PRINTER_H

#include "Expr.h"
#include "Stmt.h"
#include "Value.h"

class RPN_Printer : public Expr::Visitor, public Stmt::Visitor {

  using RETURN_TYPE = Expr::RETURN_TYPE;

public:
  RPN_Printer();
  virtual ~RPN_Printer() = default;

  shared_ptr<String> print(shared_ptr<Stmt> stmt);
  shared_ptr<String> print(shared_ptr<Expr> expr);
  shared_ptr<String> print(const vector<shared_ptr<Stmt>> &stmts);
  shared_ptr<String> print(const vector<shared_ptr<Expr>> &exprs);
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
};

#endif /*  */
