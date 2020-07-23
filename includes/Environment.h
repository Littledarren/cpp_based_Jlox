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
        Environment(Environment *enclosing = nullptr) : 
            enclosing(enclosing){}
        ~Environment() {
            for (const std::pair<string, const Object*> &pair : values) {
                delete pair.second;
            }
        }

        void define(const string &name, const Object* value);
        const Object* get(const Token *name);
        void assign(const Token* name, const Object* value);
        Environment* enclosing;

        //const std::map<string, Object*>& getObjects() const;
        void print() const;
    private:
        std::map<const string, const Object*> values;

};

#endif
