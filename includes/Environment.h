/*================================================================
*    
*   
*   FileName: Environment.h
*   Author: DarrenHuang
*   Create Time: 2020/07/05  18:03(星期日)
*   Description:
*
================================================================*/
#ifndef _ENVIRONMENT_H_
#define _ENVIRONMENT_H_

#include <map>
#include <string>
using std::string;

#include "Interpreter.h"

class Environment
{
    public:
        Environment(){}

        void define(const string &name, VALUE_T * value);
        VALUE_T * get(Token * const  &name);
    private:
        std::map<string, VALUE_T*> values;

};

#endif
