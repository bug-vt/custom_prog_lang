#ifndef PARSER_HPP
#define PARSER_HPP

#include "lexer.hpp"
#include "token.hpp"
#include "func_table.hpp"
#include "string_table.hpp"
#include "symbol_table.hpp"
#include <fstream>
#include "expr.hpp"
#include "stmt.hpp"

class Parser
{
  public:
    Parser () { }
    Parser (std::string raw_source);
    std::vector<Stmt*> parse ();
    
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
    Token readToken (TokenType req_token);
    Stmt* parseStatement ();
    Stmt* parseExprStatement ();
    Stmt* parseDeclaration ();
    Stmt* parseVar ();
    void parseBlock ();
    void parseFunc ();
    Expr* parseExpr ();
    Expr* parseTerm ();
    Expr* parseFactor ();
    Expr* parseUnary ();
    Expr* parsePrimary ();
};

#endif
