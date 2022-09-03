#ifndef _ASM_PARSER
#define _ASM_PARSER

#include "asm_lexer.hpp"
#include "token.hpp"

class AsmParser
{
  public:
    AsmParser () { }
    AsmParser (std::string raw_source);


  private:
    AsmLexer lexer;

    void readToken (Token req_token);
};

#endif
