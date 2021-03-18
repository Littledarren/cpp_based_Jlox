/*================================================================
*
*   👌
*   FileName: Token.h
*   Author: DarrenHuang
*   Create Time: 2020/07/06  09:16(星期一)
*   Description:
*       Token  used by Lexer to produce token list
*
================================================================*/
#ifndef _TOKEN_H_
#define _TOKEN_H_

#include "Object.h"
#include "TokenType.h"

// 符号，最基础的语义单元, 类似于单词word
// 字母->单词->句子
struct Token {
  Token(TokenType type, const string &lexeme, const shared_ptr<Object> &literal,
        int line)
      : type(type), lexeme(lexeme), literal(literal), line(line) {}
  operator string() const {
    return getNameOfType(type) + "---[" + lexeme + "]---[" +
           (literal ? "VAL" : "NIL") + "]";
  }
  // 类型
  const TokenType type;
  // 名字
  const string lexeme;
  // 实际值
  const shared_ptr<Object> literal;
  // 没啥用，用于调试
  const int line;
};

#endif
