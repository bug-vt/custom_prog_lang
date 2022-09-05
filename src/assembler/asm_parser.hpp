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
    // variables for constructing header 
    int stack_size;
    bool is_main_func_present;
    int main_func_index;
    int global_data_size;
    int instr_stream_size;
    // variables for tracking current function
    int curr_scope;
    int curr_func_local_data_size;
    int curr_func_param_size;

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
    void parseVar ();
    void parseParam ();
};

#endif
