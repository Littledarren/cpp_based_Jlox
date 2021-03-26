/*================================================================

*
*
*   FileName: ErrorReporting.h
*   Author: DarrenHuang
*   Create Time: 2021/03/23  16:27(星期二)
*   Description:
*
================================================================*/

#ifndef ERRORREPORTING_H
#define ERRORREPORTING_H

#include "Token.h"
#include <iostream>
#include <stdexcept>

namespace clox {

using std::cerr;
using std::endl;
using std::runtime_error;
extern bool hadError; // has error ?
extern bool hadRuntimeError;

namespace error {
using token::Token;
////////////////////////////////////////////////////////////////////////
//                           Runtime Error                            //
////////////////////////////////////////////////////////////////////////

class RuntimeError : public runtime_error {
public:
  RuntimeError(const shared_ptr<const Token> &token, const string &str)
      : runtime_error(str), token(token) {}

  shared_ptr<const Token> token;
};
////////////////////////////////////////////////////////////////////////
//                          Compiling Error                           //
////////////////////////////////////////////////////////////////////////

class ParseError : public std::runtime_error {
public:
  ParseError(const string &s) : runtime_error(s) {}
};
////////////////////////////////////////////////////////////////////////
//                          ERROR REPORTING                           //
////////////////////////////////////////////////////////////////////////

inline void report(int line, const string &where, const string &message) {
  cerr << "[" << line << "] Error " << where << " : " << message << endl;
  hadError = true;
}
inline void error(int line, const string &message) {
  report(line, "", message);
}
inline void error(const Token &token, const string &msg) {
  if (token.type == token::FOE) {
    report(token.line, "at end", msg);
  } else {
    report(token.line, "at '" + token.lexeme + "'", msg);
  }
}

inline void runtimeError(const RuntimeError &e) {
  // cerr<<e.what()<<"\n[line "<<  e.token->line <<"]";
  error(*e.token, e.what());
  hadRuntimeError = true;
}

} // namespace error
} // namespace clox

#endif /* ERRORREPORTING_H */
