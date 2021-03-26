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
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Interpreter.h"
#include "Lexer.h"
#include "Parser.h"
#include "Resolver.h"
#include "Token.h"

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;

namespace clox {

// Compilation Error
bool hadError = false; // has error ?
// Runtime Error
bool hadRuntimeError = false;

enum ERROR_CODE {

  ARGUMENT_TOO_LESS = 64,
  COMPILATION_ERROR = 65,
  RUNTIME_ERROR = 70
};
} // namespace clox

namespace {
using namespace clox;
void runFile(const char *path);
void runPrompt();
void run(const string &source);

} // namespace
using namespace clox;

int main(int argc, char *argv[]) {
  if (argc > 5) {
    cout << "=======HELP=========" << endl;
    cout << "Usage: jlox [script]" << endl;
    return clox::ARGUMENT_TOO_LESS;
  } else if (argc >= 2) {
    runFile(argv[argc - 1]);
  } else {
    runPrompt();
  }

  return 0;
}

namespace {
using namespace clox;

void runFile(const char *path) {
  string source, temp;
  ifstream is(path);
  while (getline(is, temp)) {
    source.append(temp + "\n");
  }
  run(source);
  // in c++, there is no need to do this ... disgusting thing at all.
  // is.close();
  if (clox::hadError)
    exit(clox::COMPILATION_ERROR);
  if (clox::hadRuntimeError)
    exit(clox::RUNTIME_ERROR);
}
void runPrompt() {
  string cmd;
  do {
    cout << "> ";
    if (cin.good() && getline(cin, cmd))
      run(cmd);
    clox::hadError = false;
  } while (cin.good());
}

void run(const string &source) {
  // interpreter has some information to preserve.
  static clox::runtime::Interpreter interpreter;

  // 1.词法
  clox::compiling::Lexer lexer(source);
  const vector<shared_ptr<token::Token>> tokens = lexer.scanTokens();
#ifdef DEBUG
  for (auto p : tokens) {
    cout << (string)*p << endl;
  }
#endif

  // 2.句法or语法
  compiling::Parser parser(tokens);
  vector<shared_ptr<compiling::Stmt>> statements = parser.parse();

  if (!hadError) {
    clox::compiling::Resolver resolver(interpreter);
    resolver.resolve(statements);
  }
  if (!hadError) {
    // 3/.语义
    interpreter.interprete(statements);
    // delete astP;
  }
}
} // namespace
