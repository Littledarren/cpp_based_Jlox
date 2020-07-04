#include "../includes/RecursiveDescentParser.h"



vector<Stmt*> RecursiveDescentParser::parse() 
{
    vector<Stmt*> statements;
    try {
        while (!isAtEnd()) {
            statements.push_back(statement());
        }
    } catch (const ParseError &pe) {
        return statements;
    } catch (char const * e) {
        cout<<e<<endl;
        return statements;
    }
    return statements;
}
Stmt* RecursiveDescentParser::statement()
{
    if (match({PRINT})) return printStatement();

    return expressionStatement();
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
    consume(SEMICOLON, "Expect ';' after expression");
    return new Expression(expr);
}
Expr* RecursiveDescentParser::expression()
{
    return commaExpression();
}
Expr* RecursiveDescentParser::commaExpression()
{
    Expr *expr = equality();
    while (match({COMMA})) {
        Token *op = previous();
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
        Token *op = previous();
        Expr *right = comparison();
        expr = new Binary(expr, op, right);
    }
    return expr;
}
Expr* RecursiveDescentParser::comparison()
{
    Expr *expr = addition();
    while (match({GREATER, GREATER_EQUAL, LESS_EQUAL, LESS})) {
        Token *op = previous();
        Expr *right = addition();
        expr = new Binary(expr, op, right);
    }
    return expr;
}
Expr* RecursiveDescentParser::addition()
{
    Expr *expr = multiplication();

    while (match({MINUS, PLUS})) {
        Token *op = previous();
        Expr *right = multiplication();
        expr = new Binary(expr, op, right);
    }
    return expr;
}
Expr* RecursiveDescentParser::multiplication()
{
    Expr *expr = unary();
    while (match({STAR, SLASH})) {
        Token *op = previous();
        Expr *right = unary();
        expr = new Binary(expr, op, right);
    }
    return expr;
}
Expr* RecursiveDescentParser::unary()
{
    if (match({BANG, MINUS})) {
        Token *op = previous();
        Expr *right = unary();
        return new Unary(op, right);
    }
    return primary();
}
Expr* RecursiveDescentParser::primary()
{
    if (match({FALSE, TRUE, NIL, STRING, NUMBER})) {
        return new Literal(new TokenType(previous()->type), previous()->literal);
    }
    if (match({LEFT_PAREN})) {
        Expr *expr = expression();
        consume(RIGHT_PAREN, "Expect ')' after expression");
        return new Grouping(expr);
    }
    throw error(peek(), "Expect expression.");
}
Token* RecursiveDescentParser::consume(TokenType type,const string &message)
{
    if (check(type))  return advance();
    throw error(peek(), message);
}
ParseError RecursiveDescentParser::error(Token *token, const string &message)
{
    ::error(*token, message);
    return ParseError(message);
}
void RecursiveDescentParser::synchronize()
{
    //consume the token that makes error.
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
