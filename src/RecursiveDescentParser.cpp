#include "RecursiveDescentParser.h"

#include "Value.h"
#include "main.h"


vector<shared_ptr<Stmt>> Parser::RecursiveDescentParser::parse() 
{
    vector<shared_ptr<Stmt>> statements;
    while (!isAtEnd()) {
        statements.push_back(declaration());
    }
    return statements;
}
shared_ptr<Stmt> Parser::RecursiveDescentParser::declaration()
{
    try {
        if (match({VAR})) return varDeclaration();
        //function..class..etc
        return statement();
    } catch (const ParseError &e) {
        synchronize();
        return nullptr;
    }
}
shared_ptr<Stmt> Parser::RecursiveDescentParser::varDeclaration()
{
   shared_ptr<Token> name = consume(IDENTIFIER, "Expect variable name");
   shared_ptr<Expr> initializer = nullptr;
   if (match ({EQUAL})) {
       initializer = expression();
   }
   consume(SEMICOLON, "Expect ';' after variable declaration");
   return std::make_shared<Var>(name, initializer);
}
shared_ptr<Stmt> Parser::RecursiveDescentParser::statement()
{
    if (match({PRINT})) return printStatement();
    if (match({LEFT_BRACE})) return std::make_shared<Block>(block());
    if (match({IF})) return ifStatement(); 
    if (match({WHILE})) return whileStatement();
    if (match({FOR})) return forStatement();

    return expressionStatement();
}
shared_ptr<Stmt> Parser::RecursiveDescentParser::whileStatement()
{
    consume(LEFT_PAREN, "Expect '(' after while");
    shared_ptr<Expr>condition = expression();
    consume(RIGHT_PAREN, "Expect ')' after condition");
    shared_ptr<Stmt>body = statement();
    return std::make_shared<While>(condition, body);
}
shared_ptr<Stmt> Parser::RecursiveDescentParser::ifStatement()
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

shared_ptr<Stmt> Parser::RecursiveDescentParser::forStatement()
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
   /*
    *
    * for(initializer; condition; increment)
    *       statements;
    *
    *   ---->
    *
    *   {
    *       initializer;
    *       while(condition) {
    *           statements;
    *           increment
    *       }
    *   }
    *
    *
    */
}
shared_ptr<Stmt> Parser::RecursiveDescentParser::printStatement()
{
    shared_ptr<Expr> value = expression();
    consume(SEMICOLON, "Expect ';' after value");
    return std::make_shared<Print>(value);
}

shared_ptr<Stmt> Parser::RecursiveDescentParser::expressionStatement()
{

    shared_ptr<Expr> expr = expression();
    if (match({SEMICOLON}))
        return std::make_shared<Expression>(expr);
    else 
        return std::make_shared<Print>(expr);
}

vector<shared_ptr<Stmt>> Parser::RecursiveDescentParser::block()
{
    vector<shared_ptr<Stmt>> statements;
    while (!check(RIGHT_BRACE) && !isAtEnd()) {
        statements.push_back(declaration());
    }
    consume(RIGHT_BRACE, "Expect '}' after block");
    return statements;

}
shared_ptr<Expr> Parser::RecursiveDescentParser::expression()
{
    return commaExpression();
}
shared_ptr<Expr> Parser::RecursiveDescentParser::commaExpression()
{
    shared_ptr<Expr> expr = assignment();
    while (match({COMMA})) {
        shared_ptr<Token> op = previous();
        shared_ptr<Expr>right = assignment();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}
shared_ptr<Expr> Parser::RecursiveDescentParser::assignment()
{
    //because
    //lvalue can be something like bar.foo.x
    //so we can not tell a lvalue until we found a =
    shared_ptr<Expr> expr = ternaryExpression();

    if (match({EQUAL})) {
        shared_ptr<Token> equals = previous();
        shared_ptr<Expr>value = assignment();
        //bad manner. though it works.
        if (auto temp = std::dynamic_pointer_cast<Variable>(expr)) {
            shared_ptr<Token> name = temp->name;
            return std::make_shared<Assign>(name, value);
        }

        error(equals, "Invalid assignment target");
    }
    return expr;
}
shared_ptr<Expr> Parser::RecursiveDescentParser::ternaryExpression() 
{
    shared_ptr<Expr>expr = logicalOr();
    if (match({QUESTION_MASK})) {
        shared_ptr<Expr> if_yes = ternaryExpression();
        consume(COLON, "Expect ':' in ternaryExpression");
        shared_ptr<Expr> if_no = ternaryExpression();
        expr = std::make_shared<Ternary>(expr, if_yes, if_no);
    }
    return expr; 
}
shared_ptr<Expr> Parser::RecursiveDescentParser::logicalOr()
{
    shared_ptr<Expr> expr = logicalAnd();
    while (match({OR})) {
        shared_ptr<Token> op = previous();
        shared_ptr<Expr> right = logicalAnd();
        expr = std::make_shared<Logical>(expr, op, right);
    }
    return expr;
}
shared_ptr<Expr> Parser::RecursiveDescentParser::logicalAnd()
{
    shared_ptr<Expr>expr = equality();
    while (match({AND})) {
        shared_ptr<Token> op = previous();
        shared_ptr<Expr>right = equality();
        expr = std::make_shared<Logical>(expr, op, right);
    }
    return expr;
}
shared_ptr<Expr> Parser::RecursiveDescentParser::equality()
{
    shared_ptr<Expr>expr = comparison();
    while (match({BANG_EQUAL, EQUAL_EQUAL})) {
        shared_ptr<Token> op = previous();
        shared_ptr<Expr>right = comparison();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}
shared_ptr<Expr> Parser::RecursiveDescentParser::comparison()
{
    shared_ptr<Expr>expr = addition();
    while (match({GREATER, GREATER_EQUAL, LESS_EQUAL, LESS})) {
        shared_ptr<Token> op = previous();
        shared_ptr<Expr>right = addition();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}
shared_ptr<Expr> Parser::RecursiveDescentParser::addition()
{
    shared_ptr<Expr>expr = multiplication();

    while (match({MINUS, PLUS})) {
        shared_ptr<Token> op = previous();
        shared_ptr<Expr>right = multiplication();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}
shared_ptr<Expr> Parser::RecursiveDescentParser::multiplication()
{
    shared_ptr<Expr>expr = unary();
    while (match({STAR, SLASH})) {
        shared_ptr<Token> op = previous();
        shared_ptr<Expr>right = unary();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}
shared_ptr<Expr> Parser::RecursiveDescentParser::unary()
{
    if (match({BANG, MINUS})) {
        shared_ptr<Token> op = previous();
        shared_ptr<Expr>right = unary();
        return std::make_shared<Unary>(op, right);
    }
    return call();
}
shared_ptr<Expr> Parser::RecursiveDescentParser::call()
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
shared_ptr<Expr> Parser::RecursiveDescentParser::finishCall(shared_ptr<Expr>callee)
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
    shared_ptr<Token> paren = consume(RIGHT_PAREN, "Expect ')' after arguments");
    return std::make_shared<Call>(callee, paren, arguments);
}
shared_ptr<Expr> Parser::RecursiveDescentParser::primary()
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
shared_ptr<Token> Parser::RecursiveDescentParser::consume(TokenType type,const string &message)
{
    if (check(type))  return advance();
    throw error(peek(), message);
}
ParseError Parser::RecursiveDescentParser::error(shared_ptr<Token>token, const string &message)
{
    ::error(*token, message);
    return ParseError(message);
}
void Parser::RecursiveDescentParser::synchronize()
{
    //consume the last Token that makes error.
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
