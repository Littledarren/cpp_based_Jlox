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

#include "../includes/main.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "../includes/Token.h"
#include "../includes/Lexer.h"
#include "../includes/Parser.h"
#include "../includes/Interpreter.h"

using std::ifstream;

bool hadError = false; //has error ?
bool hadRuntimeError = false;


static void runFile(const char *path);
static void runPrompt();
static void run(const string &source);

int main(int argc, char *argv[])
{

    if (argc > 2) {
        cout<<"Usage: jlox [script]"<<endl;
        return 64;
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
    is.close();
    if (hadError) exit(65);
    if (hadRuntimeError) exit(70);
}


static void runPrompt()
{
    string cmd;
    for(;;) {
        cout<<"> ";
        getline(cin, cmd);
        run(cmd);
        hadError = false;
    }
}

static void run(const string &source)
{
    // interpreter has some information to preserve.
    static Interpreter interpreter;

    //1.词法
    unique_ptr<Lexer> lexer(new Lexer(source));
    const vector<shared_ptr<Token>> &tokens = lexer->scanTokens();
    
    //2.句法or语法
    unique_ptr<Parser> parser(new Parser(tokens));
    vector<shared_ptr<Stmt>> statements = parser->parse();

    if (!hadError) {
        //3/.语义
       interpreter.interprete(statements);
        //delete astP;
    }
}



