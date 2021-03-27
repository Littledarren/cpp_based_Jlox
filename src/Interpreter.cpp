#include "Interpreter.h"

#include <exception>
#include <iostream>

#include "LoxCallable.h"
#include "LoxInstance.h"
using std::cout;

namespace clox {
namespace runtime {
using namespace error;
Interpreter::Interpreter() noexcept
    : globals(std::make_shared<Environment>(nullptr)), environment(globals) {
  environment->define("clock", std::make_shared<Clock>());
  environment->define("input", std::make_shared<Input>());
}

void Interpreter::interprete(vector<StmtPtr> statements) noexcept {
  try {
    for (auto p : statements) {
      execute(p);
    }
  } catch (const RuntimeError &e) {
    runtimeError(e);
  }
}

Interpreter::RETURN_TYPE
Interpreter::interprete(shared_ptr<Expr> expr) noexcept {
  try {
    return evaluate(expr);
  } catch (const RuntimeError &e) {
    runtimeError(e);
  } catch (...) {
    std::cout << "other exception found" << std::endl;
  }
  return nullptr;
}

Interpreter::RETURN_TYPE Interpreter::evaluate(shared_ptr<Expr> expr) {
  if (expr)
    return expr->accept(*this);
  error::error(-1, "expr nullptr");
  return nullptr;
}
void Interpreter::execute(shared_ptr<Stmt> stmt) {
  if (stmt)
    stmt->accept(*this);
}
void Interpreter::executeBlock(vector<shared_ptr<Stmt>> stmts,
                               shared_ptr<Environment> environment) {
  shared_ptr<Environment> previous = this->environment;
  try {
    this->environment = (environment);
    for (auto ptr : stmts) {
      execute(ptr);
    }
  } catch (...) {
    this->environment = previous;
    throw;
  }
  this->environment = previous;
}

////////////////////////////////////////////////////////////////////////
//                               visit                                //
////////////////////////////////////////////////////////////////////////

Interpreter::RETURN_TYPE Interpreter::visit(shared_ptr<Assign> expr) {
  RETURN_TYPE value = evaluate(expr->value);

  auto iter = locals.find(expr);
  if (iter == locals.end())
    globals->assign(expr->name, value);
  else
    environment->assignAt(iter->second, expr->name, value);
  return value;
}

Interpreter::RETURN_TYPE Interpreter::visit(shared_ptr<Logical> expr) {
  RETURN_TYPE left = evaluate(expr->left);
  if (expr->op->type == AND) {
    if (!left || !left->isTrue())
      return left;
  } else {
    if (left && left->isTrue())
      return left;
  }
  return evaluate(expr->right);
}

Interpreter::RETURN_TYPE Interpreter::visit(shared_ptr<Call> expr) {
  RETURN_TYPE callee = (evaluate(expr->callee));

  vector<RETURN_TYPE> arguments;
  for (auto p : expr->arguments) {
    arguments.push_back(evaluate(p));
  }

  shared_ptr<Callable> function = std::dynamic_pointer_cast<Callable>(callee);
  if (!function) {
    throw RuntimeError(expr->paren, "can only call functions and classes");
  }
  if (arguments.size() != function->arity()) {
    ostringstream oss;
    oss << "Expected " << function->arity() << "arguments but got"
        << arguments.size() << ".";
    throw RuntimeError(expr->paren, oss.str());
  }

  // return new Object((*callee)(arguments));
  return function->call(*this, arguments);
}

Interpreter::RETURN_TYPE Interpreter::visit(shared_ptr<Ternary> expr) {
  RETURN_TYPE cond = evaluate(expr->condition);

  if (cond && cond->isTrue()) {
    return evaluate(expr->if_yes);
  } else {
    return evaluate(expr->if_no);
  }
}
Interpreter::RETURN_TYPE Interpreter::visit(shared_ptr<Binary> expr) {
  // this will creat a new ..
  RETURN_TYPE left = evaluate(expr->left);
  RETURN_TYPE right = evaluate(expr->right);

  bool is_string = !!std::dynamic_pointer_cast<String>(left);

  RETURN_TYPE result = nullptr;

  switch (expr->op->type) {
  case PLUS:
    checkStringOrNumber(expr->op, left, right);
    //字符串可以跟其他类型+
    //要么都是数字，要么左边是字符串，没有其他可能(nil也可能。。)
    if (is_string) {
      result = value::StringAdd(std::dynamic_pointer_cast<String>(left), right);
    } else
      result =
          std::make_shared<Number>(*std::dynamic_pointer_cast<Number>(left) +
                                   *std::dynamic_pointer_cast<Number>(right));
    break;
  case MINUS:
    chechNumber(expr->op, left, right);
    result =
        std::make_shared<Number>(*std::dynamic_pointer_cast<Number>(left) -
                                 *std::dynamic_pointer_cast<Number>(right));
    break;
  case STAR:
    chechNumber(expr->op, left, right);
    result =
        std::make_shared<Number>(*std::dynamic_pointer_cast<Number>(left) *
                                 *std::dynamic_pointer_cast<Number>(right));
    break;
  case SLASH:
    chechNumber(expr->op, left, right);
    result =
        std::make_shared<Number>(*std::dynamic_pointer_cast<Number>(left) /
                                 *std::dynamic_pointer_cast<Number>(right));
    break;
  case COMMA:
    return right;
    break;
  case GREATER:
    chechNumber(expr->op, left, right);
    result = std::make_shared<Bool>(*std::dynamic_pointer_cast<Number>(left) >
                                    *std::dynamic_pointer_cast<Number>(right));
    break;
  case GREATER_EQUAL:
    chechNumber(expr->op, left, right);
    result = std::make_shared<Bool>(*std::dynamic_pointer_cast<Number>(left) >=
                                    *std::dynamic_pointer_cast<Number>(right));
    break;
  case LESS:
    chechNumber(expr->op, left, right);
    result = std::make_shared<Bool>(*std::dynamic_pointer_cast<Number>(left) <
                                    *std::dynamic_pointer_cast<Number>(right));
    break;
  case LESS_EQUAL:
    chechNumber(expr->op, left, right);
    result = std::make_shared<Bool>(*std::dynamic_pointer_cast<Number>(left) <=
                                    *std::dynamic_pointer_cast<Number>(right));
    break;
  case EQUAL_EQUAL:
    result = std::make_shared<Bool>(*left == *right);
    break;
  case BANG_EQUAL:
    result = std::make_shared<Bool>(!(*left == *right));
    break;
  default:
    throw string("ERROR UNKONE OP") + expr->op->lexeme;
  }
  return result;
}
Interpreter::RETURN_TYPE Interpreter::visit(shared_ptr<Unary> expr) {
  RETURN_TYPE temp = evaluate(expr->right);
  RETURN_TYPE result = nullptr;
  switch (expr->op->type) {
  case MINUS:
    result =
        std::make_shared<Number>(-*std::dynamic_pointer_cast<Number>(temp));
    break;
  case BANG:
    result = std::make_shared<Bool>(!*std::dynamic_pointer_cast<Bool>(temp));
    break;
  case PLUS:
    result = std::make_shared<Number>(*std::dynamic_pointer_cast<Number>(temp));
    break;
  default:
    throw RuntimeError(expr->op, "r u kidding me with a wrong unary op?");
  }
  return result;
}
Interpreter::RETURN_TYPE Interpreter::visit(shared_ptr<Grouping> expr) {
  return evaluate(expr->expr);
}
Interpreter::RETURN_TYPE Interpreter::visit(shared_ptr<Literal> expr) {
  return expr->value;
}
Interpreter::RETURN_TYPE Interpreter::visit(shared_ptr<Variable> expr) {
  return lookUpVariable(expr->name, expr);
}

Interpreter::RETURN_TYPE Interpreter::visit(shared_ptr<Lambda> expr) {
  return std::make_shared<LoxFunction>(
      std::make_shared<Function>(nullptr, expr->shared_from_this(),
                                 FunctionType::FUNCTION),
      environment);
}
Interpreter::RETURN_TYPE Interpreter::visit(shared_ptr<Get> expr) {
  shared_ptr<LoxInstance> obj =
      std::dynamic_pointer_cast<LoxInstance>(evaluate(expr->expr));
  if (obj) {
    return obj->get(expr->name);
  }
  throw RuntimeError(expr->name, "Only instances have properties");
}
Interpreter::RETURN_TYPE Interpreter::visit(shared_ptr<Set> expr) {
  shared_ptr<LoxInstance> obj =
      std::dynamic_pointer_cast<LoxInstance>(evaluate(expr->obj));
  if (obj) {
    auto value = evaluate(expr->value);
    obj->set(expr->token, value);
    return value;
  }
  throw RuntimeError(expr->token, "Only instances have fields");
}
Interpreter::RETURN_TYPE Interpreter::visit(shared_ptr<This> expr) {
  return lookUpVariable(expr->keyword, expr);
}
Interpreter::RETURN_TYPE Interpreter::visit(shared_ptr<Super> expr) {
  int dis = locals.at(expr);
  auto super_class =
      std::dynamic_pointer_cast<LoxClass>(environment->getAt(dis, "super"));
  auto instance = std::dynamic_pointer_cast<LoxInstance>(
      environment->getAt(dis - 1, "this"));
  auto method = super_class->findMethod(expr->method->lexeme);
  if (!method)
    throw RuntimeError(expr->method,
                       "Undefined property '" + expr->method->lexeme + "'");
  return method->bind(instance);
}
void Interpreter::visit(shared_ptr<Expression> stmt) { evaluate(stmt->expr); }
void Interpreter::visit(shared_ptr<Print> stmt) {
  RETURN_TYPE value = evaluate(stmt->expr);
  if (value)
    std::cout << (value)->toString() << endl;
  else
    std::cout << "nil" << endl;
}

void Interpreter::visit(shared_ptr<Var> stmt) {
  RETURN_TYPE value = nullptr;
  if (stmt->initializer != nullptr) {
    value = evaluate(stmt->initializer);
  }
  this->environment->define(stmt->name->lexeme, value);
}
void Interpreter::visit(shared_ptr<Block> stmt) {
  executeBlock(stmt->statements, std::make_shared<Environment>(environment));
}

void Interpreter::visit(shared_ptr<If> stmt) {
  RETURN_TYPE cond = evaluate(stmt->condition);
  if (cond && cond->isTrue()) {
    execute(stmt->thenBranch);
  } else if (stmt->elseBranch) {
    execute(stmt->elseBranch);
  }
}

void Interpreter::printEnvironment() {
  shared_ptr<Environment> temp = this->environment;
  int scopeCount = 0;
  while (temp) {
    cout << "======SCOPE:" << scopeCount << "======" << endl;
    temp->print();
    temp = temp->enclosing;
  }
}

void Interpreter::visit(shared_ptr<While> stmt) {
  RETURN_TYPE p = nullptr;
  while ((p = evaluate(stmt->condition)) && p->isTrue()) {
    execute(stmt->body);
  }
}
void Interpreter::visit(shared_ptr<Function> func) {
  environment->define(
      func->name->lexeme,
      std::make_shared<LoxFunction>(func->shared_from_this(), environment));
}

void Interpreter::visit(shared_ptr<Return> stmt) {
  RETURN_TYPE value = nullptr;
  if (stmt->value) {
    value = evaluate(stmt->value);
  }
  //如果返回函数，需要hold Environment
  if (auto func = std::dynamic_pointer_cast<LoxFunction>(value)) {
    value = func->hold();
  }
  throw Control(stmt->name->type, value);
}
void Interpreter::visit(shared_ptr<Class> stmt) {
  shared_ptr<LoxClass> super_class;

  if (stmt->super_class) {
    super_class =
        std::dynamic_pointer_cast<LoxClass>(evaluate(stmt->super_class));
    if (!super_class) {
      throw RuntimeError(stmt->super_class->name,
                         "Super class must be a class.");
    }
  }

  environment->define(stmt->name->lexeme, nullptr);

  if (super_class) {
    environment = std::make_shared<Environment>(environment);
    environment->define("super", super_class);
  }
  std::map<string, shared_ptr<LoxFunction>> methods;
  for (auto &method : stmt->methods) {
    auto mm = std::make_shared<LoxFunction>(method, environment);
    methods[method->name->lexeme] = mm->hold();
  }

  auto kclass =
      std::make_shared<LoxClass>(stmt->name->lexeme, super_class, methods);
  if (super_class) {
    environment = environment->enclosing;
  }
  environment->assign(stmt->name, kclass);
}
Interpreter::RETURN_TYPE Interpreter::lookUpVariable(shared_ptr<Token> name,
                                                     shared_ptr<Expr> key) {
  auto iter = locals.find(key);
  if (iter == locals.end())
    return globals->get(name);
  else
    return environment->getAt(iter->second, name->lexeme);
}
void Interpreter::checkStringOrNumber(shared_ptr<Token> op, RETURN_TYPE l,
                                      RETURN_TYPE r) {
  if (std::dynamic_pointer_cast<Number>(l) &&
      std::dynamic_pointer_cast<Number>(r))
    return;
  if (std::dynamic_pointer_cast<String>(l))
    return;
  throw RuntimeError(op, "operands should be numbers or strings ");
}
void Interpreter::chechNumber(shared_ptr<Token> op, shared_ptr<Object> v) {
  if (std::dynamic_pointer_cast<Number>(v))
    return;
  throw RuntimeError(op, "Operand must be a number");
}
void Interpreter::chechNumber(shared_ptr<Token> op, shared_ptr<Object> l,
                              shared_ptr<Object> r) {
  if (std::dynamic_pointer_cast<Number>(l) &&
      std::dynamic_pointer_cast<Number>(r))
    return;
  throw RuntimeError(op, "operands should be numbers");
}
} // namespace runtime
} // namespace clox
