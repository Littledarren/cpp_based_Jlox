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

#include "Expr.h"
#include "Stmt.h"
#include "TokenType.h"
#include "Value.h"

#include "Environment.h"
using std::vector;

typedef shared_ptr<Stmt> StmtPtr;

using RETURN_TYPE = shared_ptr<Object>;

// tree walker
class Interpreter : public Expr::Visitor, public Stmt::Visitor
{

public:
    Interpreter():environment(new Environment()){}
    void interprete(vector<StmtPtr> statements);

    RETURN_TYPE interprete(shared_ptr<Expr>expr);
    RETURN_TYPE evaluate(shared_ptr<Expr>expr);
    void execute(shared_ptr<Stmt>  stmt);
    void executeBlock(vector<StmtPtr> stmts, shared_ptr<Environment> environment);
    //expr
    RETURN_TYPE visit(const Assign &expr) override;
    RETURN_TYPE visit(const Binary &expr) override;
    RETURN_TYPE visit(const Grouping &expr) override;
    RETURN_TYPE visit(const Literal &expr) override;
    RETURN_TYPE visit(const Unary &expr) override;
    RETURN_TYPE visit(const Ternary &expr) override;
    RETURN_TYPE visit(const Logical &expr) override;
    RETURN_TYPE visit(const Variable &expr) override;
    RETURN_TYPE visit(const Call &expr) override;
    //Stmt
    void visit(const Expression &stmt) override;
    void visit(const Print &stmt) override;
    void visit(const Var &stmt) override;
    void visit(const Block &stmt) override;
    void visit(const If &stmt) override;
    void visit(const While &stmt) override;

    //aditional funcs for debug
    void printEnvironment();
private:
    //check operant type
    static void checkStringOrNumber(shared_ptr<Token>op, RETURN_TYPE l, RETURN_TYPE r);
    static void chechNumber(shared_ptr<Token>op, RETURN_TYPE v);
    static void chechNumber(shared_ptr<Token>op, RETURN_TYPE l, RETURN_TYPE r);
    shared_ptr<Environment> environment;

};

#endif
