#ifndef _ERROR
#define _ERROR

#include <string>
#include "asm_lexer.hpp"

void exitOnError (std::string msg);
void exitOnCodeError (std::string msg, AsmLexer &lexer);

#endif
