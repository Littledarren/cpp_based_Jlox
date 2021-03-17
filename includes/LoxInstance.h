/*================================================================
*    
*   
*   FileName: LoxInstance.h
*   Author: DarrenHuang
*   Create Time: 2021/03/17  19:21(星期三)
*   Description:
*
================================================================*/
#ifndef LOXINSTANCE_H
#define LOXINSTANCE_H

#include "Object.h"
#include "LoxCallable.h"
#include "RuntimeError.h"

#include <map>

class LoxInstance : public virtual Object
{
    using FIELD_TYPE = shared_ptr<Object>;
public:
    LoxInstance(LoxClass &klass) : klass(klass)
    {

    }
    virtual ~LoxInstance()=default;
    virtual string toString() const override
    {
        return klass.name + "  instance";
    }
    FIELD_TYPE get(shared_ptr<Token> token)
    {
        if (fields.find(token->lexeme) != fields.end())
            return fields.at(token->lexeme);

        throw RuntimeError(token, "Undefined property '" + token->lexeme + "'");
    }
    void set(shared_ptr<Token> token, FIELD_TYPE value)
    {
        fields[token->lexeme] = value;
    }
private:
    LoxClass &klass; 
    std::map<string, FIELD_TYPE> fields;
};

#endif /* LOXINSTANCE_H */
