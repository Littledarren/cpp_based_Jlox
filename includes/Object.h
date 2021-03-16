/*================================================================
*    
*   
*   FileName: Object.h
*   Author: DarrenHuang
*   Create Time: 2020/07/22  20:51(Wednesday)
*   Description:
*
================================================================*/
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <string>
#include <memory>
using std::string;
using std::shared_ptr;

struct Object
{
    virtual bool operator ==(const Object &o) const;
    virtual string toString() const;
    virtual shared_ptr<Object> clone() const;
    virtual bool isTrue() const;
    virtual ~Object(){}

};

#endif
