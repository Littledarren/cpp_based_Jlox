#ifndef LEXER_H
#define LEXER_H
#include <memory>
#include <vector>
using std::shared_ptr;
using std::unique_ptr;
using std::vector;

#include "Token.h"

// interface for lexical parsing
class Lexer {
public:
  vector<shared_ptr<Token>> scanTokens();
  Lexer(const string &source);
  // ERROR, no complete LexerImp here
  //~Lexer()=default;
  ~Lexer();

private:
  class LexerImp;
  unique_ptr<LexerImp> impl;
};

#endif
