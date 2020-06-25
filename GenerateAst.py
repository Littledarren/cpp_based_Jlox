#!/bin/python3.8
# coding=utf-8 

import sys

print(sys.argv)
if len(sys.argv) != 2:
    sys.stderr.write("Usage: generate_ast <output directory>") 
    exit(64)

outputDir = sys.argv[1]

def defineVisitor(out_file, baseName, types):
    out_file.write("template<typename R>\n struct Visitor {\n")

    for t in types:
        typeName = t.split(":")[0].strip()
        out_file.write("virtual R visit" + typeName +baseName +"("+
                typeName+ " " + baseName.lower() + ")=0;")
    pass
   
    out_file.write(" };")



def defineAst(outputDir, baseName, types):
    path = outputDir + "/" + baseName + ".h"
    
    with open(path, "wt") as out_file:
        out_file.write('''
#ifndef _EXPR_H_
#define _EXPR_H_

#include <iostream>
#include "TokenType.h"
#include "Token.h"
using namespace std;
''')
#        out_file.write("#ifndef _EXPR_H_\n")
#        out_file.write("#define _EXPR_H_\n")
#        out_file.write("\n")
#        out_file.write("#include<iostream>\n")
#        out_file.write("#include\"TokenType.h\"\n")
#        out_file.write("#include\"Token.h\"\n")
        
        out_file.write("struct {}\n{{\ntemplate<typename R>\nvirtual R  accept(Visitor<R> visitor)=0;\n}};\n".format(baseName))

        #define visitor
        defineVisitor(out_file, baseName, types)

        #define subclass
        for t in types:
            className = t.split(':')[0].strip()
            fields = t.split(':')[1].strip()
            defineType(out_file, baseName, className, fields)

        out_file.write("\n#endif")


def defineType(out_file, baseName, className, fields):

    #struc start
    out_file.write("struct {}:public {}\n {{\n".format(className, baseName))

    fieldList = [[y.strip() for y in x.strip().split(" ")] for x in fields.split(",")]
    print(fieldList)
    #constructor
    out_file.write("    {}({}):".format(className, fields))
    for i, field in enumerate(fieldList):
        N = field[1]
        # pointer is so anti-human
        N = N[1:]
        out_file.write("{}({}){}".format(N, N, "," if i != len(fieldList)-1 else ""))
    pass
    out_file.write("\n{}\n")
    #deconsructor
    out_file.write("~{}()\n{{\n".format(className))

    if className == 'Literal':
        out_file.write('''
        switch(*type)
        {
            case STRING:
                delete (string*)value;
                break;
            case NUMBER:
                delete (double*)value;
                break;
            default:
                std::cout<<"RUNTIME ERROR WHEN DELETING " <<getNameOfType(*type)<<std::endl;
        }
        delete type;
    }
''')
    else:
        for field in fieldList:
            N = field[1]
            N = N[1:]
            out_file.write("delete {};\n".format(N))

        out_file.write("\n}\n")

    out_file.write("template<typename R>\n R accept(Visitor<R> visitor) override {")
    out_file.write("return visitor.visit" + className + baseName + "(this);\n")
    out_file.write("}\n")
    

    #define data 
    for field in fieldList:
        T = field[0]
        N = field[1]
        out_file.write("const {} {};\n".format(T, N))

    #struct end
    out_file.write("};\n")






defineAst(outputDir, "Expr", [          
    "Binary   : Expr *left, Token *op, Expr *right",
    "Grouping : Expr *expression",                      
    "Literal  : TokenType *type, void *value",                         
    "Unary    : Token *op, Expr *right"            
    ]) 








