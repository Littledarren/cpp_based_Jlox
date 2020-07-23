#include "../includes/RecursiveDescentParser.h"



vector<Stmt*> RecursiveDescentParser::parse() 
{
    vector<Stmt*> statements;
    while (!isAtEnd()) {
        statements.push_back(declaration());
    }
    return statements;
}
Stmt* RecursiveDescentParser::declaration()
{
    try {
        if (match({VAR})) return varDeclaration();
        return statement();
    } catch (const ParseError &e) {
        synchronize();
        return nullptr;
    }
}
Stmt* RecursiveDescentParser::varDeclaration()
{
   const Token* name = consume(IDENTIFIER, "Expect variable name");
   Expr * initializer = nullptr;
   if (match ({EQUAL})) {
       initializer = expression();
   }
   consume(SEMICOLON, "Expect ';' after variable declaration");
   return new Var(name, initializer);
}
Stmt* RecursiveDescentParser::statement()
{
    if (match({PRINT})) return printStatement();
    if (match({LEFT_BRACE})) return new Block(block());
    if (match({IF})) return ifStatement(); 
    if (match({WHILE})) return whileStatement();
    if (match({FOR})) return forStatement();

    return expressionStatement();
}
Stmt* RecursiveDescentParser::whileStatement()
{
    consume(LEFT_PAREN, "Expect '(' after while");
    Expr *condition = expression();
    consume(RIGHT_PAREN, "Expect ')' after condition");
    Stmt *body = statement();
    return new While(condition, body);
}
Stmt* RecursiveDescentParser::ifStatement()
{
    consume(LEFT_PAREN, "Expect '(' after 'if'");
    Expr *condition = expression();
    consume(RIGHT_PAREN, "Expect ')' after if condition");

    Stmt *thenBranch = statement();
    Stmt *elseBranch = nullptr;
    if (match({ELSE})) {
        elseBranch = statement();
    }

    return new If(condition, thenBranch, elseBranch);
}

Stmt* RecursiveDescentParser::forStatement()
{
   consume(LEFT_PAREN, "Expect a '(' after for");

   //1.
   Stmt *initializer;
   if (match({SEMICOLON})) {
       initializer = nullptr;
   } else if (match({VAR})) {
       initializer = varDeclaration();
   } else {
       initializer = expressionStatement();
   }
   //2.
   Expr *condition = nullptr;
   if (!check(SEMICOLON)) {
       condition = expression();
   }
   consume(SEMICOLON, "Expect ';' after loop condition");
   //3.
   Expr *increment = nullptr;
   if (!check(RIGHT_PAREN)) {
       increment = expression();
   }
   consume(RIGHT_PAREN, "Expect ')' after for clauses");

   Stmt *body = statement();
   if (increment != nullptr) {
       body = new Block({body, new Expression(increment)});
   }
   if (condition == nullptr) 
       condition = new Literal(new Bool(true));

   body = new While(condition, body);
   if (initializer != nullptr)
       body = new Block({initializer, body});
   return body;
}
Stmt* RecursiveDescentParser::printStatement()
{
    Expr* value = expression();
    consume(SEMICOLON, "Expect ';' after value");
    return new Print(value);
}

Stmt* RecursiveDescentParser::expressionStatement()
{

    Expr* expr = expression();
    if (match({SEMICOLON}))
        return new Expression(expr);
    else 
        return new Print(expr);
}

vector<Stmt*> RecursiveDescentParser::block()
{
    vector<Stmt*> statements;
    while (!check(RIGHT_BRACE) && !isAtEnd()) {
        statements.push_back(declaration());
    }
    consume(RIGHT_BRACE, "Expect '}' after block");
    return statements;

}
Expr* RecursiveDescentParser::expression()
{
    return assignment();
}
Expr* RecursiveDescentParser::assignment()
{
    //because
    //lvalue can be something like bar.foo.x
    //so we can not tell a lvalue until we found a =
    Expr *expr = logicalOr();

    if (match({EQUAL})) {
        const Token* equals = previous();
        Expr *value = assignment();
        //bad manner. though it works.
        if (typeid(*expr) == typeid(Variable)) {
            const Token* name = dynamic_cast<Variable*>(expr)->name;
            return new Assign(name, value);
        }

        error(equals, "Invalid assignment target");
    }
    return expr;
}
Expr* RecursiveDescentParser::logicalOr()
{
    Expr *expr = logicalAnd();
    while (match({OR})) {
        const Token* op = previous();
        Expr *right = logicalAnd();
        expr = new Logical(expr, op, right);
    }
    return expr;
}
Expr* RecursiveDescentParser::logicalAnd()
{
    Expr *expr = equality();
    while (match({AND})) {
        const Token* op = previous();
        Expr *right = equality();
        expr = new Logical(expr, op, right);
    }
    return expr;
}
Expr* RecursiveDescentParser::commaExpression()
{
    Expr *expr = equality();
    while (match({COMMA})) {
        const Token* op = previous();
        Expr *right = equality();
        expr = new Binary(expr, op, right);
    }
    return expr;
}
Expr* RecursiveDescentParser::ternaryExpression()
{
    //Expr *expr = equality();
    return nullptr; 

}
Expr* RecursiveDescentParser::equality()
{
    Expr *expr = comparison();
    while (match({BANG_EQUAL, EQUAL_EQUAL})) {
        const Token* op = previous();
        Expr *right = comparison();
        expr = new Binary(expr, op, right);
    }
    return expr;
}
Expr* RecursiveDescentParser::comparison()
{
    Expr *expr = addition();
    while (match({GREATER, GREATER_EQUAL, LESS_EQUAL, LESS})) {
        const Token* op = previous();
        Expr *right = addition();
        expr = new Binary(expr, op, right);
    }
    return expr;
}
Expr* RecursiveDescentParser::addition()
{
    Expr *expr = multiplication();

    while (match({MINUS, PLUS})) {
        const Token* op = previous();
        Expr *right = multiplication();
        expr = new Binary(expr, op, right);
    }
    return expr;
}
Expr* RecursiveDescentParser::multiplication()
{
    Expr *expr = unary();
    while (match({STAR, SLASH})) {
        const Token* op = previous();
        Expr *right = unary();
        expr = new Binary(expr, op, right);
    }
    return expr;
}
Expr* RecursiveDescentParser::unary()
{
    if (match({BANG, MINUS})) {
        const Token* op = previous();
        Expr *right = unary();
        return new Unary(op, right);
    }
    return call();
}
Expr* RecursiveDescentParser::call()
{
   Expr *expr = primary(); 

   while (true) {
       if (match({LEFT_PAREN})) {
           expr = finishCall(expr);
       } else {
           break;
       }
   }

   return expr;
}
Expr* RecursiveDescentParser::finishCall(Expr *callee)
{
    vector<Expr*> arguments;
    if (!check(RIGHT_PAREN)) {
        do {
            if (arguments.size() >= 255) {
                error(peek(), "can not have more than 255 arguments");
            }
            arguments.push_back(expression());
        } while (match({COMMA}));
    }
    const Token* paren = consume(RIGHT_PAREN, "Expect ')' after arguments");
    return new Call(callee, paren, arguments);
}
Expr* RecursiveDescentParser::primary()
{
    if (match({FALSE, TRUE}))
        return new Literal(new Bool(previous()->type == TRUE));
    if (match({NIL}))
        return new Literal(nullptr);
    if (match({STRING, NUMBER}))
        return new Literal(previous()->literal);
    if (match({LEFT_PAREN})) {
        Expr *expr = expression();
        consume(RIGHT_PAREN, "Expect ')' after expression");
        return new Grouping(expr);
    }
    //变量使用
    if (match({IDENTIFIER})) {
        return new Variable(previous());
    }
    throw error(peek(), "Expect expression.");
}
const Token* RecursiveDescentParser::consume(TokenType type,const string &message)
{
    if (check(type))  return advance();
    throw error(peek(), message);
}
ParseError RecursiveDescentParser::error(const Token *token, const string &message)
{
    ::error(*token, message);
    return ParseError(message);
}
void RecursiveDescentParser::synchronize()
{
    //consume the const Token that makes error.
    advance();

    while (!isAtEnd()) {
        //find where a statement ends
        if (previous()->type == SEMICOLON) return;

        switch(peek()->type) {
            case CLASS:
            case FUN:
            case VAR:
            case FOR:
            case WHILE:
            case IF:
            case PRINT:
            case RETURN:
                return;
            default:
                break;
        }
        advance();
    }
}
