/*================================================================
*    
*   
*   FileName: main.cpp
*   Author: DarrenHuang
*   Create Time: 2020/06/14  19:43(Sunday)
*   Description:
*       As we shall know,
*       source code ---Lexer-->  Tokens ----Parser---> statements
*       ----interpreter--->output
*       This is interpretation.
*
================================================================*/
#define DEBUG
#include "main.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Token.h"
#include "Lexer.h"
#include "Parser.h"
#include "Interpreter.h"

using std::ifstream;
using std::cin;
using std::cout;


// Compilation Error
bool hadError = false; //has error ?
// Runtime Error
bool hadRuntimeError = false;

enum ERROR_CODE 
{

    ARGUMENT_TOO_LESS = 64,
    COMPILATION_ERROR = 65,
    RUNTIME_ERROR = 70
};


static void runFile(const char *path);
static void runPrompt();
static void run(const string &source);

int main(int argc, char *argv[])
{

    if (argc > 2) {
        cout<<"Usage: jlox [script]"<<endl;
        return ARGUMENT_TOO_LESS;
    } else if (argc == 2) {
        runFile(argv[1]);
    } else {
        runPrompt();
    }

    return 0;
}



static void runFile(const char *path)
{
    string source, temp;
    ifstream is(path); 
    while (getline(is, temp)) {
        source.append(temp + "\n");
    }
    run(source);
    //in c++, there is no need to do this ... disgusting thing at all.
    //is.close();
    if (hadError) exit(COMPILATION_ERROR);
    if (hadRuntimeError) exit(RUNTIME_ERROR);
}


static void runPrompt()
{
    string cmd;
    do {
        cout<<"> ";
        if (cin.good() && getline(cin, cmd))
            run(cmd);
        hadError = false;
    } while(cin.good());
}

static void run(const string &source)
{
    // interpreter has some information to preserve.
    static Interpreter interpreter;

    //1.词法
    Lexer lexer(source);
    const vector<shared_ptr<Token>> tokens = lexer.scanTokens();
#ifdef DEBUG
    for (auto p : tokens) {
        cout<<(string)*p<<endl;
    }
#endif
    
    //2.句法or语法
    Parser parser(tokens);
    vector<shared_ptr<Stmt>> statements = parser.parse();

    if (!hadError) {
        //3/.语义
       interpreter.interprete(statements);
        //delete astP;
    }
}



