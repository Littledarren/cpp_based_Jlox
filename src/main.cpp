/*================================================================
*    
*   
*   FileName: main.cpp
*   Author: DarrenHuang
*   Create Time: 2020/06/14  19:43(Sunday)
*   Description:
*
================================================================*/


#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "../includes/main.h"
#include "../includes/Lexer.h"
#include "../includes/Scanner.h"
#include "../includes/Token.h"
#include "../includes/AstPrinter.h"
#include "../includes/RecursiveDescentParser.h"
#include "../includes/Interpreter.h"

using namespace std;


bool hadError = false; //has error ?
bool hadRuntimeError = false;


static void runFile(const char *path);
static void runPrompt();
static void run(const string &source);


static Interpreter interpreter;

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
    string cmd;
    ifstream is(path); 
    is>>cmd;
    run(cmd);
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
    //abstract!!!!!!!!!
    Lexer *lexer = new Scanner(source);
    vector<Token*> tokens = lexer->scanTokens();
    Parser *parser = new RecursiveDescentParser(tokens);

    vector<Stmt*> statements = parser->parse();
    if (!hadError) {
        cout<<"执行中"<<endl;
//        AstPrinter *astP = new AstPrinter();
//        cout<<astP->print(expr)<<endl;
       // VALUE_T result = interpreter.interprete(expr);
       // cout<<VALUE_T::toString(result)<<endl;
       interpreter.interprete(statements);
        //delete astP;
    }
    delete lexer;
    delete parser;
}



