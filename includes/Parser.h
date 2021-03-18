/*================================================================
*
*
*   FileName: Parser.h
*   Author: DarrenHuang
*   Create Time: 2020/06/19  10:53(Friday)
*   Description:
*
================================================================*/
#ifndef _PARSER_H_
#define _PARSER_H_

#include <memory>
#include <vector>

#include "Stmt.h"

using std::unique_ptr;
using std::vector;

// construct abstract syntax tree
class Parser {
public:
  Parser(const vector<shared_ptr<Token>> &tokens);
  vector<shared_ptr<Stmt>> parse();
  ~Parser();

private:
  class RecursiveDescentParser;
  unique_ptr<RecursiveDescentParser> impl;
};

#endif
