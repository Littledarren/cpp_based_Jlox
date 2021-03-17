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

class LoxInstance : public virtual Object
{
public:
    LoxInstance(LoxClass &klass) : klass(klass)
    {

    }
    virtual ~LoxInstance()=default;
    virtual string toString() const override
    {
        return klass.name + "  instance";
    }
private:
    LoxClass &klass; 
};

#endif /* LOXINSTANCE_H */
