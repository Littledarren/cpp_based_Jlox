/*================================================================
*    
*   
*   FileName: RecursiveDescentParser.h
*   Author: DarrenHuang
*   Create Time: 2020/06/19  11:00(Friday)
*   Description:
*
================================================================*/
#ifndef _RECURSIVEDESCENTPARSER_H_
#define _RECURSIVEDESCENTPARSER_H_

#include <initializer_list>
#include <exception>
#include "Parser.h"
#include "Expr.h"
#include "main.h"

class ParseError : public std::runtime_error
{
public:
    ParseError(const string &s):
        runtime_error(s)
    {
    }
};

// a implementation of Parser
class RecursiveDescentParser : public Parser
{
public:

    RecursiveDescentParser(vector<Token*> &tokens) : Parser(tokens)
    {}
    virtual Expr* parse() override 
    {
        try {
            return expression();
        } catch (ParseError *pe) {
            delete pe;
            return nullptr;
        } catch (char const * e) {
            cout<<e<<endl;
            return nullptr;
        }
    }
private:
    Expr* expression()
    {
        return commaExpression();
    }
    Expr* commaExpression()
    {
        Expr *expr = equality();
        while (match({COMMA})) {
            Token *op = previous();
            Expr *right = equality();
            expr = new Binary(expr, op, right);
        }
        return expr;
    }
    Expr* ternaryExpression()
    {
        //Expr *expr = equality();
        return nullptr; 

    }
    Expr* equality()
    {
        Expr *expr = comparison();
        while (match({BANG_EQUAL, EQUAL_EQUAL})) {
            Token *op = previous();
            Expr *right = comparison();
            expr = new Binary(expr, op, right);
        }
        return expr;
    }
    Expr* comparison()
    {
        Expr *expr = addition();
        while (match({GREATER, GREATER_EQUAL, LESS_EQUAL, LESS})) {
            Token *op = previous();
            Expr *right = addition();
            expr = new Binary(expr, op, right);
        }
        return expr;
    }
    Expr* addition()
    {
        Expr *expr = multiplication();

        while (match({MINUS, PLUS})) {
            Token *op = previous();
            Expr *right = multiplication();
            expr = new Binary(expr, op, right);
        }
        return expr;
    }
    Expr* multiplication()
    {
        Expr *expr = unary();
        while (match({STAR, SLASH})) {
            Token *op = previous();
            Expr *right = unary();
            expr = new Binary(expr, op, right);
        }
        return expr;
    }
    Expr* unary()
    {
        if (match({BANG, MINUS})) {
            Token *op = previous();
            Expr *right = unary();
            return new Unary(op, right);
        }
        return primary();
    }
    Expr* primary()
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
    Token* consume(TokenType type,const string &message)
    {
        if (check(type))  return advance();
        throw error(peek(), message);
    }
    ParseError* error(Token *token, const string &message)
    {
        ::error(*token, message);
        return new ParseError(message);
    }
    void synchronize()
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
    bool match(std::initializer_list<TokenType> types)
    {
        //toleratable types list
        for (auto t : types) {
            if (check(t)) {
                advance();
                return true;
            }
        }
        return false;

    }
    bool check(TokenType t) const
    {
        if (isAtEnd()) return false;
        return peek()->type == t;
    }
    Token* advance()
    {
        if (!isAtEnd()) current++;
        return previous();
    }

    bool isAtEnd() const
    {
        return peek()->type == FOE;
    }

    Token* peek() const
    {
        return tokens.at(current);
    }
    Token* previous() const
    {
        return tokens.at(current-1);
    }

    int current = 0;

};

#endif
