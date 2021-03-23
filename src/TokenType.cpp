/*================================================================
*
*
*   FileName: TokenType.cpp
*   Author: DarrenHuang
*   Create Time: 2021/03/23  17:22(星期二)
*   Description:
*
================================================================*/

#include "TokenType.h"

namespace clox {
namespace token {

std::unordered_map<TokenType, string> mapper{// Single character tokens
                                             {NUL, "NUL"},
                                             {LEFT_PAREN, "LEFT_PAREN"},
                                             {RIGHT_PAREN, "RIGHT_PAREN"},
                                             {LEFT_BRACE, "LEFT_BRACE"},
                                             {RIGHT_BRACE, "RIGHT_BRACE"},
                                             {COMMA, "COMMA"},
                                             {DOT, "DOT"},
                                             {MINUS, "MINUS"},
                                             {PLUS, "PLUS"},
                                             {SLASH, "SLASH"},
                                             {STAR, "STAR"},
                                             {SEMICOLON, "SEMICOLON"},
                                             {QUESTION_MASK, "QUESTION_MASK"},
                                             {COLON, "COLON"},

                                             {BANG, "BANG"},
                                             {BANG_EQUAL, "BANG_EQUAL"},
                                             {EQUAL, "EQUAL"},
                                             {EQUAL_EQUAL, "EQUAL_EQUAL"},
                                             {GREATER, "GREATER"},
                                             {GREATER_EQUAL, "GREATER_EQUAL"},
                                             {LESS, "LESS"},
                                             {LESS_EQUAL, "LESS_EQUAL"},

                                             // {literals,"literals"}
                                             {IDENTIFIER, "IDENTIFIER"},
                                             {STRING, "STRING"},
                                             {NUMBER, "NUMBER"},

                                             // {keywords,"keywords"}
                                             {AND, "AND"},
                                             {CLASS, "CLASS"},
                                             {ELSE, "ELSE"},
                                             {FALSE, "FALSE"},
                                             {FUN, "FUN"},
                                             {FOR, "FOR"},
                                             {IF, "IF"},
                                             {NIL, "NIL"},
                                             {OR, "OR"},
                                             {PRINT, "PRINT"},
                                             {RETURN, "RETURN"},
                                             {SUPER, "SUPER"},
                                             {THIS, "THIS"},
                                             {TRUE, "TRUE"},
                                             {VAR, "VAR"},
                                             {WHILE, "WHILE"},
                                             {CONTINUE, "CONTINUE"},
                                             {BREAK, "BREAK"},

                                             {FOE, "FOE"}};
} // namespace token
} // namespace clox
