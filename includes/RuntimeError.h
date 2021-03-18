/*================================================================
class RuntimeError : public std::runtime_error
*
*
*   FileName: RuntimeError.h
*   Author: DarrenHuang
*   Create Time: 2020/07/06  08:36(星期一)
*   Description:
*
================================================================*/
#ifndef _RUNTIMEERROR_H_
#define _RUNTIMEERROR_H_

#include <stdexcept>
using std::runtime_error;

#include "Token.h"

class RuntimeError : public runtime_error {
public:
  RuntimeError(shared_ptr<const Token> token, const string &str)
      : runtime_error(str), token(token) {}

  shared_ptr<const Token> token;
};
#endif
