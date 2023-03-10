#ifndef _ASM_PARSER
#define _ASM_PARSER

#include "asm_lexer.hpp"
#include "token.hpp"
#include "func_table.hpp"
#include "string_table.hpp"
#include "symbol_table.hpp"
#include "label_table.hpp"
#include "instruction.hpp"
#include "code_gen.hpp"
#include <fstream>


class AsmParser
{
  public:
    AsmParser () { }
    AsmParser (std::string raw_source);
    void parse ();
    void parse (int pass_id);
    CodeGen createCodeGen (std::ostream &out_file); 
    
  private:
    // keep track of the which pass is on 
    int curr_pass;
    // variables for tracking current function
    int curr_scope;
    int curr_func_local_data_size;
    int curr_func_param_size;

    // lexer
    AsmLexer lexer;

    // storing header info
    int stack_size;
    bool is_main_func_present;
    int main_func_index;
    int global_data_size;
    // storing instruction stream
    std::vector<Instr> instr_stream;
    int instr_count;
    // tables
    InstrLookupTable instr_table = InstrLookupTable::instance ();
    FuncTable func_table;
    StringTable str_table;
    SymbolTable symbol_table;
    LabelTable label_table;

    // methods
    void readToken (Token req_token);
    void parseLine ();
    // 1st pass
    void parseFunc ();
    void parseBlock ();
    void parseVar ();
    void parseLabel ();
    // 2nd pass
    void parseParam ();
    void parseInstr ();
};

#endif
