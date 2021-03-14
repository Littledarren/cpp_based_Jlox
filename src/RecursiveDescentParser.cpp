#include "../includes/RecursiveDescentParser.h"



vector<shared_ptr<Stmt>> RecursiveDescentParser::parse() 
{
    vector<shared_ptr<Stmt>> statements;
    while (!isAtEnd()) {
        statements.push_back(declaration());
    }
    return statements;
}
shared_ptr<Stmt> RecursiveDescentParser::declaration()
{
    try {
        if (match({VAR})) return varDeclaration();
        return statement();
    } catch (const ParseError &e) {
        synchronize();
        return nullptr;
    }
}
shared_ptr<Stmt> RecursiveDescentParser::varDeclaration()
{
   shared_ptr<const Token> name = consume(IDENTIFIER, "Expect variable name");
   shared_ptr<Expr> initializer = nullptr;
   if (match ({EQUAL})) {
       initializer = expression();
   }
   consume(SEMICOLON, "Expect ';' after variable declaration");
   return std::make_shared<Var>(name, initializer);
}
shared_ptr<Stmt> RecursiveDescentParser::statement()
{
    if (match({PRINT})) return printStatement();
    if (match({LEFT_BRACE})) return std::make_shared<Block>(block());
    if (match({IF})) return ifStatement(); 
    if (match({WHILE})) return whileStatement();
    if (match({FOR})) return forStatement();

    return expressionStatement();
}
shared_ptr<Stmt> RecursiveDescentParser::whileStatement()
{
    consume(LEFT_PAREN, "Expect '(' after while");
    shared_ptr<Expr>condition = expression();
    consume(RIGHT_PAREN, "Expect ')' after condition");
    shared_ptr<Stmt>body = statement();
    return std::make_shared<While>(condition, body);
}
shared_ptr<Stmt> RecursiveDescentParser::ifStatement()
{
    consume(LEFT_PAREN, "Expect '(' after 'if'");
    shared_ptr<Expr>condition = expression();
    consume(RIGHT_PAREN, "Expect ')' after if condition");

    shared_ptr<Stmt>thenBranch = statement();
    shared_ptr<Stmt>elseBranch = nullptr;
    if (match({ELSE})) {
        elseBranch = statement();
    }
    return std::make_shared<If>(condition, thenBranch, elseBranch);

}

shared_ptr<Stmt> RecursiveDescentParser::forStatement()
{
   consume(LEFT_PAREN, "Expect a '(' after for");

   //1.
   shared_ptr<Stmt>initializer;
   if (match({SEMICOLON})) {
       initializer = nullptr;
   } else if (match({VAR})) {
       initializer = varDeclaration();
   } else {
       initializer = expressionStatement();
   }
   //2.
   shared_ptr<Expr>condition = nullptr;
   if (!check(SEMICOLON)) {
       condition = expression();
   }
   consume(SEMICOLON, "Expect ';' after loop condition");
   //3.
   shared_ptr<Expr>increment = nullptr;
   if (!check(RIGHT_PAREN)) {
       increment = expression();
   }
   consume(RIGHT_PAREN, "Expect ')' after for clauses");

   shared_ptr<Stmt>body = statement();
   if (increment) {
       body = std::make_shared<Block>(vector<shared_ptr<Stmt>>({body, std::make_shared<Expression>(increment)}));
   }
   if (!condition) 
       condition = std::make_shared<Literal>(std::make_shared<Bool>(true));

   body = std::make_shared<While>(condition, body);
   if (initializer)
       body = std::make_shared<Block>(vector<shared_ptr<Stmt>>({initializer, body}));
   return body;
}
shared_ptr<Stmt> RecursiveDescentParser::printStatement()
{
    shared_ptr<Expr> value = expression();
    consume(SEMICOLON, "Expect ';' after value");
    return std::make_shared<Print>(value);
}

shared_ptr<Stmt> RecursiveDescentParser::expressionStatement()
{

    shared_ptr<Expr> expr = expression();
    if (match({SEMICOLON}))
        return std::make_shared<Expression>(expr);
    else 
        return std::make_shared<Print>(expr);
}

vector<shared_ptr<Stmt>> RecursiveDescentParser::block()
{
    vector<shared_ptr<Stmt>> statements;
    while (!check(RIGHT_BRACE) && !isAtEnd()) {
        statements.push_back(declaration());
    }
    consume(RIGHT_BRACE, "Expect '}' after block");
    return statements;

}
shared_ptr<Expr> RecursiveDescentParser::expression()
{
    return assignment();
}
shared_ptr<Expr> RecursiveDescentParser::assignment()
{
    //because
    //lvalue can be something like bar.foo.x
    //so we can not tell a lvalue until we found a =
    shared_ptr<Expr>expr = logicalOr();

    if (match({EQUAL})) {
        shared_ptr<const Token> equals = previous();
        shared_ptr<Expr>value = assignment();
        //bad manner. though it works.
        if (auto temp = std::dynamic_pointer_cast<Variable>(expr)) {
            shared_ptr<const Token> name = temp->name;
            return std::make_shared<Assign>(name, value);
        }

        error(equals, "Invalid assignment target");
    }
    return expr;
}
shared_ptr<Expr> RecursiveDescentParser::logicalOr()
{
    shared_ptr<Expr> expr = logicalAnd();
    while (match({OR})) {
        shared_ptr<const Token> op = previous();
        shared_ptr<Expr> right = logicalAnd();
        expr = std::make_shared<Logical>(expr, op, right);
    }
    return expr;
}
shared_ptr<Expr> RecursiveDescentParser::logicalAnd()
{
    shared_ptr<Expr>expr = equality();
    while (match({AND})) {
        shared_ptr<const Token> op = previous();
        shared_ptr<Expr>right = equality();
        expr = std::make_shared<Logical>(expr, op, right);
    }
    return expr;
}
shared_ptr<Expr> RecursiveDescentParser::commaExpression()
{
    shared_ptr<Expr>expr = equality();
    while (match({COMMA})) {
        shared_ptr<const Token> op = previous();
        shared_ptr<Expr>right = equality();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}
shared_ptr<Expr> RecursiveDescentParser::ternaryExpression() 
{
    //shared_ptr<Expr>expr = equality();
    return nullptr; 
}
shared_ptr<Expr> RecursiveDescentParser::equality()
{
    shared_ptr<Expr>expr = comparison();
    while (match({BANG_EQUAL, EQUAL_EQUAL})) {
        shared_ptr<const Token> op = previous();
        shared_ptr<Expr>right = comparison();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}
shared_ptr<Expr> RecursiveDescentParser::comparison()
{
    shared_ptr<Expr>expr = addition();
    while (match({GREATER, GREATER_EQUAL, LESS_EQUAL, LESS})) {
        shared_ptr<const Token> op = previous();
        shared_ptr<Expr>right = addition();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}
shared_ptr<Expr> RecursiveDescentParser::addition()
{
    shared_ptr<Expr>expr = multiplication();

    while (match({MINUS, PLUS})) {
        shared_ptr<const Token> op = previous();
        shared_ptr<Expr>right = multiplication();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}
shared_ptr<Expr> RecursiveDescentParser::multiplication()
{
    shared_ptr<Expr>expr = unary();
    while (match({STAR, SLASH})) {
        shared_ptr<const Token> op = previous();
        shared_ptr<Expr>right = unary();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}
shared_ptr<Expr> RecursiveDescentParser::unary()
{
    if (match({BANG, MINUS})) {
        shared_ptr<const Token> op = previous();
        shared_ptr<Expr>right = unary();
        return std::make_shared<Unary>(op, right);
    }
    return call();
}
shared_ptr<Expr> RecursiveDescentParser::call()
{
   shared_ptr<Expr>expr = primary(); 

   while (true) {
       if (match({LEFT_PAREN})) {
           expr = finishCall(expr);
       } else {
           break;
       }
   }

   return expr;
}
shared_ptr<Expr> RecursiveDescentParser::finishCall(shared_ptr<Expr>callee)
{
    vector<shared_ptr<Expr>> arguments;
    if (!check(RIGHT_PAREN)) {
        do {
            if (arguments.size() >= 255) {
                error(peek(), "can not have more than 255 arguments");
            }
            arguments.push_back(expression());
        } while (match({COMMA}));
    }
    shared_ptr<const Token> paren = consume(RIGHT_PAREN, "Expect ')' after arguments");
    return std::make_shared<Call>(callee, paren, arguments);
}
shared_ptr<Expr> RecursiveDescentParser::primary()
{
    if (match({FALSE, TRUE}))
        return std::make_shared<Literal>(std::make_shared<Bool>(previous()->type == TokenType::TRUE));
    if (match({NIL}))
        return std::make_shared<Literal>(nullptr);
    if (match({STRING, NUMBER}))
        return std::make_shared<Literal>(previous()->literal);
    if (match({LEFT_PAREN})) {
        shared_ptr<Expr>expr = expression();
        consume(RIGHT_PAREN, "Expect ')' after expression");
        return std::make_shared<Grouping>(expr);
    }
    //变量使用
    if (match({IDENTIFIER})) {
        return std::make_shared<Variable>(previous());
    }
    throw error(peek(), "Expect expression.");
}
shared_ptr<const Token> RecursiveDescentParser::consume(TokenType type,const string &message)
{
    if (check(type))  return advance();
    throw error(peek(), message);
}
ParseError RecursiveDescentParser::error(shared_ptr<const Token>token, const string &message)
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
