#ifndef PARSER_HPP
#define PARSER_HPP

#include "lexer.hpp"
#include "token.hpp"
#include "func_table.hpp"
#include "string_table.hpp"
#include "symbol_table.hpp"
#include "icode.hpp"
#include "code_gen.hpp"
#include <fstream>
#include "expr.hpp"

class Parser
{
  public:
    Parser () { }
    Parser (std::string raw_source);
    void parse ();
    CodeGen createCodeGen (); 
    
  private:
    // variables for tracking current function
    int curr_scope;

    // lexer
    Lexer lexer;

    // general-purpose registers
    int tmp0_sym_index;
    int tmp1_sym_index;

    // tables
    FuncTable func_table;
    StringTable str_table;
    SymbolTable symbol_table;

    // methods
    void readToken (Token req_token);
    void parseStatement ();
    void parseBlock ();
    void parseFunc ();
    void parseVar ();
    void parseExpr ();
    void parseSubExpr ();
};

#endif
