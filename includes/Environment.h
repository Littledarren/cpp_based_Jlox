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
#include <memory>

#include "../includes/Value.h"
class Token;

using std::string;
using std::shared_ptr;

class Environment
{
    public:
        Environment(shared_ptr<Environment> enclosing = nullptr) : 
            enclosing(enclosing){}

        void define(const string &name, shared_ptr<const Object> value);
        shared_ptr<const Object> get(shared_ptr<const Token> name);
        void assign(shared_ptr<const Token> name, shared_ptr<const Object> value);
        shared_ptr<Environment> enclosing;

        //const std::map<string, Object*>& getObjects() const;
        void print() const;
    private:
        std::map<const string, shared_ptr<const Object>> values;

};

#endif
