#include "../includes/Scanner.h"


std::map<string, TokenType> Scanner::keywords = {
                        {"and", AND},
                        {"class", CLASS},
                        {"else", ELSE},
                        {"false", FALSE},
                        {"for", FOR},
                        {"fun", FUN},
                        {"if", IF},
                        {"nil", NIL},
                        {"or", OR},
                        {"print", PRINT},
                        {"return", RETURN},
                        {"super", SUPER},
                        {"this", THIS},
                        {"true", TRUE},
                        {"var", VAR},
                        {"while", WHILE},
};
const vector<shared_ptr<Token>>& Scanner::scanTokens()
{
    while (!isAtEnd()) {
        start = current;
        scanToken();
    }
    tokens_list.push_back(std::make_shared<Token>(TokenType::FOE, "", nullptr, line));
    
    return tokens_list;
}

bool Scanner::isAtEnd()
{
    return current >= source.length();
}
void Scanner::scanToken()
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
void Scanner::takeIdentifier()
{
    while (isAlphaNumeric(peek())) advance();
    string text = source.substr(start, current-start);
    TokenType type = keywords[text];
    if (type == NUL) type = IDENTIFIER;
    addToken(type);
}
bool Scanner::isAlpha(char c)
{
    return (c >= 'a' && c <= 'z') ||
        (c >= 'A' && c <= 'Z') ||
        (c == '_');
}
bool Scanner::isAlphaNumeric(char c)
{
    return isAlpha(c) || isDigit(c);
}
bool Scanner::isDigit(char c)
{
    return c >= '0' && c<='9';
}
void Scanner::takeNumber()
{
    while(isDigit(peek())) advance();

    //look for a fraction part
    if (peek() == '.' && isDigit(peekNext())) {
        advance();
        while (isDigit(peek())) advance();
    }
    double val; 
    std::istringstream is(source.substr(start, current-start));
    is>>val;

    addToken(NUMBER, std::make_shared<Number>(val));
}
char Scanner::peekNext()
{
    if (current + 1 >= source.length()) return '\0';
    return source.at(current+1);
}
void Scanner::takeString()
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

    string value = source.substr(start+1, current - 1 - start - 1);
    addToken(STRING, std::make_shared<String>(value));
}
char Scanner::peek() 
{
    if (isAtEnd()) return '\0';

    return source.at(current);
}
bool Scanner::match(char expected)
{
    if (isAtEnd()) return false;

    if (source.at(current) != expected) return false;
    current++;
    return true;
}
char Scanner::advance()
{
    current++;
    return source.at(current-1);
}
void Scanner::addToken(TokenType type)
{
    addToken(type, nullptr);
}
void Scanner::addToken(TokenType type, shared_ptr<const Object> literal)
{
    string text = source.substr(start, current-start);
    tokens_list.push_back(std::make_shared<Token>(type, text, literal, line));
}
