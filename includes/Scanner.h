/*================================================================
*    
*   
*   FileName: Scanner.h
*   Author: DarrenHuang
*   Create Time: 2020/06/14  18:59(Sunday)
*   Description:
*
================================================================*/
#ifndef _SCANNER_H_
#define _SCANNER_H_

#include "Lexer.h"
#include "main.h"

#include <sstream>
#include <map>

class Scanner : public Lexer
{
public:
    Scanner(const string &source):
        Lexer(source)
    {

    }
    virtual vector<Token*> scanTokens() override
    {
        while (!isAtEnd()) {
            start = current;
            scanToken();
        }
        tokens.push_back(new Token(TokenType::FOE, "", nullptr, line));


        return tokens;
    }
private:
    bool isAtEnd()
    {
        return current >= source.length();
    }
    void scanToken()
    {
        char c = advance();
        switch(c) {
            case '(': addToken(TokenType::LEFT_PAREN);break;
            case ')': addToken(TokenType::RIGHT_PAREN);break;
            case '{': addToken(TokenType::LEFT_BRACE);break;
            case '}': addToken(TokenType::RIGHT_BRACE);break;
            case ',': addToken(TokenType::COMMA);break;
            case '.': addToken(TokenType::DOT);break;
            case '-': addToken(TokenType::MINUS);break;
            case '+': addToken(TokenType::PLUS);break;
            case ';': addToken(TokenType::SEMICOLON);break;
            case '*': addToken(TokenType::STAR);break;
            case '!': addToken(match('=')?TokenType::BANG_EQUAL:BANG);break;
            case '=': addToken(match('=')?EQUAL_EQUAL:EQUAL);break;
            case '<': addToken(match('=')?LESS_EQUAL:LESS);break;
            case '>': addToken(match('=')?GREATER_EQUAL:GREATER);break;
            case '/':
                      if (match('/')) {
                          while (peek() != '\n' && !isAtEnd()) advance();
                      } else if (match('*')) {
                          while (peek() != '*' && !isAtEnd() && peekNext() != '/') {
                               if (peek() == '\n') line++;
                               advance();
                          }
                          advance();
                          advance();

                      } else {
                          addToken(SLASH);
                      }
                      break;
            case ' ':
            case '\r':
            case '\t':
                      break;
            case '\n':
                      line++; 
                      break;
            case '"': takeString();break;
            default:
                      if (isDigit(c)) {
                          takeNumber();
                      } else if (isAlpha(c)) {
                          takeIdentifier();
                      } else {
                          error(line, string("Unexpected character") + string(1, c));
                      }
                      break;

        }

    }
    void takeIdentifier()
    {
        while (isAlphaNumeric(peek())) advance();
        string text = source.substr(start, current-start);
        TokenType type = keywords[text];
        if (type == NUL) type = IDENTIFIER;
        addToken(type);
    }
    bool isAlpha(char c)
    {
        return (c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
            (c == '_');
    }
    bool isAlphaNumeric(char c)
    {
        return isAlpha(c) || isDigit(c);
    }
    bool isDigit(char c)
    {
        return c >= '0' && c<='9';
    }
    void takeNumber()
    {
        while(isDigit(peek())) advance();

        //look for a fraction part
        if (peek() == '.' && isDigit(peekNext())) {
            advance();
            while (isDigit(peek())) advance();
        }
        double *val = new double;
        std::istringstream is(source.substr(start, current-start));
        is>>*val;

        addToken(NUMBER, val);
    }
    char peekNext()
    {
        if (current + 1 >= source.length()) return '\0';
        return source.at(current+1);
    }
    void takeString()
    {
        while (peek() != '"' && !isAtEnd()) {
            if (peek() == '\n') line++;
            advance();
        }

        if (isAtEnd()) {
            error(line, "Unterminated string");
            return;
        }
        //take the "
        advance();

        string *value = new string(source.substr(start+1, current - 1 - start - 1));
        addToken(STRING, value);
    }
    char peek() 
    {
        if (isAtEnd()) return '\0';

        return source.at(current);
    }
    bool match(char expected)
    {
        if (isAtEnd()) return false;

        if (source.at(current) != expected) return false;
        current++;
        return true;
    }
    char advance() {
        current++;
        return source.at(current-1);
    }
    void addToken(TokenType type)
    {
        addToken(type, nullptr);
    }
    void addToken(TokenType type, void *literal)
    {
        string text = source.substr(start, current-start);
        tokens.push_back(new Token(type, text, literal, line));
    }
    //new feature! after..c++11?
    int start = 0;
    int current = 0;
    int line = 1;
    static std::map<string, TokenType> keywords;
};

#endif
