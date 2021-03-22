#include "RPN_Printer.h"
#include "Value.h"

/*
 * 好像有点难啊。。。很多难以定义。。。
 *
 */

shared_ptr<String> RPN_Printer::print(shared_ptr<Stmt> stmt) {
  stmt->accept(*this);
  return std::make_shared<String>("");
}

shared_ptr<String> RPN_Printer::print(shared_ptr<Expr> expr) {
  if (expr)
    return std::dynamic_pointer_cast<String>(expr->accept(*this));
  else
    return std::make_shared<String>("");
}
shared_ptr<String> RPN_Printer::print(const vector<shared_ptr<Expr>> &exprs) {
  shared_ptr<String> result = std::make_shared<String>("");
  for (auto &expr : exprs) {
    result =
        std::make_shared<String>(result->toString() + print(expr)->toString());
  }

  return result;
}
shared_ptr<String> RPN_Printer::print(const vector<shared_ptr<Stmt>> &stmts) {
  shared_ptr<String> result = std::make_shared<String>("");
  for (auto &stmt : stmts) {
    result =
        std::make_shared<String>(result->toString() + print(stmt)->toString());
  }

  return result;
}
// expr
RPN_Printer::RETURN_TYPE RPN_Printer::visit(shared_ptr<Assign> expr) {
  return std::make_shared<String>("= " + expr->name->lexeme + " " +
                                  print(expr->value)->toString());
}
RPN_Printer::RETURN_TYPE RPN_Printer::visit(shared_ptr<Binary> expr) {
  return std::make_shared<String>(expr->op->lexeme + " " +
                                  print(expr->left)->toString() + " " +
                                  print(expr->right)->toString());
}
RPN_Printer::RETURN_TYPE RPN_Printer::visit(shared_ptr<Grouping> expr) {
  return print(expr->expr);
}
RPN_Printer::RETURN_TYPE RPN_Printer::visit(shared_ptr<Literal> expr) {
  return expr->value;
}
RPN_Printer::RETURN_TYPE RPN_Printer::visit(shared_ptr<Unary> expr) {
  return std::make_shared<String>(expr->op->lexeme +
                                  print(expr->right)->toString());
}
RPN_Printer::RETURN_TYPE RPN_Printer::visit(shared_ptr<Ternary> expr) {
  return std::make_shared<String>("?:" + print(expr->condition)->toString() +
                                  " " + print(expr->if_yes)->toString() + " " +
                                  print(expr->if_no)->toString());
}
RPN_Printer::RETURN_TYPE RPN_Printer::visit(shared_ptr<Logical> expr) {
  return std::make_shared<String>(expr->op->lexeme + " " +
                                  print(expr->left)->toString() + " " +
                                  print(expr->right)->toString());
}
RPN_Printer::RETURN_TYPE RPN_Printer::visit(shared_ptr<Variable> expr) {
  return std::make_shared<String>(expr->name->lexeme);
}
RPN_Printer::RETURN_TYPE RPN_Printer::visit(shared_ptr<Call> expr) {
  return std::make_shared<String>(print(expr->callee)->toString() + " " +
                                  print(expr->arguments)->toString());
}
RPN_Printer::RETURN_TYPE RPN_Printer::visit(shared_ptr<Lambda> expr) {
  return std::make_shared<String>("lambda Function");
}
RPN_Printer::RETURN_TYPE RPN_Printer::visit(shared_ptr<Get> expr) {
  return std::make_shared<String>(print(expr->expr)->toString());
}
RPN_Printer::RETURN_TYPE RPN_Printer::visit(shared_ptr<Set> expr) {
  return std::make_shared<String>(print(expr->obj)->toString());
}
RPN_Printer::RETURN_TYPE RPN_Printer::visit(shared_ptr<This> expr) {
  return std::make_shared<String>("This");
}
// Stmt
void RPN_Printer::visit(shared_ptr<Expression> stmt) {}
void RPN_Printer::visit(shared_ptr<Print> stmt) {}
void RPN_Printer::visit(shared_ptr<Var> stmt) {}
void RPN_Printer::visit(shared_ptr<Block> stmt) {}
void RPN_Printer::visit(shared_ptr<If> stmt) {}
void RPN_Printer::visit(shared_ptr<Function> func) {}
void RPN_Printer::visit(shared_ptr<While> stmt) {}
void RPN_Printer::visit(shared_ptr<Return> stmt) {}
void RPN_Printer::visit(shared_ptr<Class> stmt) {}
