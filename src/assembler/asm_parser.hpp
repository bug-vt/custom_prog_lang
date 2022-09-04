#ifndef _ASM_PARSER
#define _ASM_PARSER

#include "asm_lexer.hpp"
#include "token.hpp"
#include "func_table.hpp"
#include "string_table.hpp"
#include "symbol_table.hpp"
#include "label_table.hpp"

class AsmParser
{
  public:
    AsmParser () { }
    AsmParser (std::string raw_source);
    void parse ();

  private:
    int curr_scope;

    // lexer
    AsmLexer lexer;

    // tables
    FuncTable func_table;
    StringTable str_table;
    SymbolTable symbol_table;
    LabelTable label_table;

    // methods
    void readToken (Token req_token);
    void parseLine ();
    void parseFunc ();
    void parseBlock ();
};

#endif
