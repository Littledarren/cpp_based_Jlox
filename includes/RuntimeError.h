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

#include "TokenType.h"

class RuntimeError : public runtime_error
{
public:
    RuntimeError(TokenType type, const char *str) : runtime_error(str), type(type)
    {}
    
    const TokenType type;
};
#endif
