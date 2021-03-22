/*================================================================
*
*
*   FileName: Lexer.cpp
*   Author: DarrenHuang
*   Create Time: 2020/10/19  23:58(Monday)
*   Description:
*
================================================================*/
#include "Lexer.h"

#include <map>
#include <sstream>

#include "Value.h"
#include "main.h"

class Lexer::LexerImp {
public:
  vector<shared_ptr<Token>> scanTokens();
  LexerImp(const string &source) : source(source) {}

private:
  bool isAtEnd();
  void scanToken();
  void takeIdentifier();
  bool isAlpha(char c);
  bool isAlphaNumeric(char c);
  bool isDigit(char c);
  void takeNumber();
  char peekNext();
  void takeString();
  char peek();
  bool match(char expected);
  char advance();
  void addToken(TokenType type);
  void addToken(TokenType type, shared_ptr<Object> literal);

private:
  // new feature! after..c++11?
  const string &source;
  vector<shared_ptr<Token>> tokens_list;
  int start = 0;
  int current = 0;
  int line = 1;
  //不能是const,因为找不到的时候会自动插值
  static std::map<string, TokenType> keywords;
};

std::map<string, TokenType> Lexer::LexerImp::keywords = {
    {"and", AND},     {"class", CLASS},   {"else", ELSE},
    {"false", FALSE}, {"for", FOR},       {"fun", FUN},
    {"if", IF},       {"nil", NIL},       {"or", OR},
    {"print", PRINT}, {"return", RETURN}, {"super", SUPER},
    {"this", THIS},   {"true", TRUE},     {"var", VAR},
    {"while", WHILE}, {"break", BREAK},   {"continue", CONTINUE}};

vector<shared_ptr<Token>> Lexer::LexerImp::scanTokens() {
  while (!isAtEnd()) {
    start = current;
    scanToken();
  }
  tokens_list.push_back(
      std::make_shared<Token>(TokenType::FOE, "", nullptr, line));

  return tokens_list;
}

bool Lexer::LexerImp::isAtEnd() { return current >= source.length(); }
void Lexer::LexerImp::scanToken() {
  char c = advance();
  switch (c) {
  case '(':
    addToken(TokenType::LEFT_PAREN);
    break;
  case ')':
    addToken(TokenType::RIGHT_PAREN);
    break;
  case '{':
    addToken(TokenType::LEFT_BRACE);
    break;
  case '}':
    addToken(TokenType::RIGHT_BRACE);
    break;
  case ',':
    addToken(TokenType::COMMA);
    break;
  case '.':
    addToken(TokenType::DOT);
    break;
  case '-':
    addToken(TokenType::MINUS);
    break;
  case '+':
    addToken(TokenType::PLUS);
    break;
  case ';':
    addToken(TokenType::SEMICOLON);
    break;
  case '*':
    addToken(TokenType::STAR);
    break;
  case '?':
    addToken(TokenType::QUESTION_MASK);
    break;
  case ':':
    addToken(TokenType::COLON);
    break;
  case '!':
    addToken(match('=') ? TokenType::BANG_EQUAL : BANG);
    break;
  case '=':
    addToken(match('=') ? EQUAL_EQUAL : EQUAL);
    break;
  case '<':
    addToken(match('=') ? LESS_EQUAL : LESS);
    break;
  case '>':
    addToken(match('=') ? GREATER_EQUAL : GREATER);
    break;
  case '/':
    if (match('/')) {
      while (peek() != '\n' && !isAtEnd())
        advance();
    } else if (match('*')) {
      while (!isAtEnd() && !(peek() == '*' && peekNext() == '/')) {
        if (peek() == '\n')
          line++;
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
  case '"':
    takeString();
    break;
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
void Lexer::LexerImp::takeIdentifier() {
  while (isAlphaNumeric(peek()))
    advance();
  string text = source.substr(start, current - start);
  TokenType type = keywords[text];
  if (type == NUL)
    type = IDENTIFIER;
  addToken(type);
}
bool Lexer::LexerImp::isAlpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_');
}
bool Lexer::LexerImp::isAlphaNumeric(char c) {
  return isAlpha(c) || isDigit(c);
}
bool Lexer::LexerImp::isDigit(char c) { return c >= '0' && c <= '9'; }
void Lexer::LexerImp::takeNumber() {
  while (isDigit(peek()))
    advance();

  // look for a fraction part
  if (peek() == '.' && isDigit(peekNext())) {
    advance();
    while (isDigit(peek()))
      advance();
  }
  double val;
  std::istringstream is(source.substr(start, current - start));
  is >> val;

  addToken(NUMBER, std::make_shared<Number>(val));
}
char Lexer::LexerImp::peekNext() {
  if (current + 1 >= source.length())
    return '\0';
  return source.at(current + 1);
}
void Lexer::LexerImp::takeString() {
  while (peek() != '"' && !isAtEnd()) {
    if (peek() == '\n')
      line++;
    advance();
  }

  if (isAtEnd()) {
    error(line, "Unterminated string");
    return;
  }
  // take the "
  advance();

  string value = source.substr(start + 1, current - 1 - start - 1);
  addToken(STRING, std::make_shared<String>(value));
}
char Lexer::LexerImp::peek() {
  if (isAtEnd())
    return '\0';

  return source.at(current);
}
bool Lexer::LexerImp::match(char expected) {
  if (isAtEnd())
    return false;

  if (source.at(current) != expected)
    return false;
  current++;
  return true;
}
char Lexer::LexerImp::advance() {
  current++;
  return source.at(current - 1);
}
void Lexer::LexerImp::addToken(TokenType type) { addToken(type, nullptr); }
void Lexer::LexerImp::addToken(TokenType type, shared_ptr<Object> literal) {
  string text = source.substr(start, current - start);
  tokens_list.push_back(std::make_shared<Token>(type, text, literal, line));
}

vector<shared_ptr<Token>> Lexer::scanTokens() { return impl->scanTokens(); }

Lexer::Lexer(const string &source) : impl(new LexerImp(source)) {}
Lexer::~Lexer() = default;
