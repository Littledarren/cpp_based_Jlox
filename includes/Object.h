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

using std::string;

struct Object
{
    virtual ~Object(){}
    virtual bool operator ==(const Object &o) const;
    virtual string toString() const;

    static Object* clone(const Object* o);
    static string toString(const Object* o);
};

#endif
