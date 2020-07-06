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

#include "../includes/Value.h"
class Token;

class Environment
{
    public:
        Environment(){}
        ~Environment() {
            for (const std::pair<string, Value*> &pair : values) {
                delete pair.second;
            }
        }

        void define(const string &name, Value *value);
        Value * get(Token *name);
        void assign(Token *name, Value *value);
    private:
        std::map<string, Value*> values;

};

#endif
