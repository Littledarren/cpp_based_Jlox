/*================================================================
*
*
*   FileName: TokenType.h *   Author: DarrenHuang
*   Create Time: 2020/07/06  09:20(星期一)
*   Description:
*           Define TokenType as the basic description for Lexer to use.
*
================================================================*/
#ifndef _TOKENTYPE_H_
#define _TOKENTYPE_H_

#include <string>
#include <unordered_map>

namespace clox {
using std::string;
using std::unordered_map;
namespace token {
enum TokenType {
  // Single character tokens
  NUL,
  LEFT_PAREN,
  RIGHT_PAREN,
  LEFT_BRACE,
  RIGHT_BRACE,
  COMMA,
  DOT,
  MINUS,
  PLUS,
  SLASH,
  STAR,
  SEMICOLON,
  QUESTION_MASK,
  COLON,

  // one or two character tokens
  BANG,
  BANG_EQUAL,
  EQUAL,
  EQUAL_EQUAL,
  GREATER,
  GREATER_EQUAL,
  LESS,
  LESS_EQUAL,

  // literals
  IDENTIFIER,
  STRING,
  NUMBER,

  // keywords
  AND,
  CLASS,
  ELSE,
  FALSE,
  FUN,
  FOR,
  IF,
  NIL,
  OR,
  PRINT,
  RETURN,
  SUPER,
  THIS,
  TRUE,
  VAR,
  WHILE,
  CONTINUE,
  BREAK,

  FOE
};

extern unordered_map<TokenType, string> mapper;

inline string getNameOfType(const TokenType &type) {
  if (mapper.find(type) != mapper.end())
    return mapper.at(type);
  return "NOT FOUND";
}
} // namespace token
} // namespace clox

#endif
