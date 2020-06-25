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

using namespace std;


bool hadError = false; //has error ?


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
    string cmd;
    ifstream is(path); 
    is>>cmd;
    run(cmd);
    is.close();
    if (hadError) exit(65);
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

    Expr *expr = parser->parse();
    if (expr != nullptr) {
        AstPrinter *astP = new AstPrinter();
        Evaluator *evaluator = new Evaluator();
        Calculator *calculator = new Calculator();
        cout<<astP->print(expr)<<endl;
        cout<<evaluator->RPN(expr)<<endl;
        cout<<calculator->eval(expr)<<endl;
        delete astP;
        delete evaluator;
        delete calculator;
    }
    delete lexer;
    delete parser;
    delete expr;
}



