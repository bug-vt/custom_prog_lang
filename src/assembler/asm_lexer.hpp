#ifndef _ASM_LEXER
#define _ASM_LEXER

#include <string>
#include <map>
#include "instr_lookup.hpp"

typedef int Token;
enum TokenType {TOKEN_TYPE_EOF,
                TOKEN_TYPE_INT,
                TOKEN_TYPE_FLOAT,
                TOKEN_TYPE_STRING,
                TOKEN_TYPE_IDENT,
                TOKEN_TYPE_COLON,
                TOKEN_TYPE_OPEN_BRACKET,
                TOKEN_TYPE_CLOSE_BRACKET,
                TOKEN_TYPE_COMMA,
                TOKEN_TYPE_OPEN_BRACE,
                TOKEN_TYPE_CLOSE_BRACE,
                TOKEN_TYPE_NEWLINE,
                TOKEN_TYPE_INSTR,
                TOKEN_TYPE_SETSTACKSIZE,
                TOKEN_TYPE_VAR,
                TOKEN_TYPE_FUNC,
                TOKEN_TYPE_PARAM,
                TOKEN_TYPE_REG_RETVAL,
                TOKEN_TYPE_INVALID};

class AsmLexer
{
  public:
    AsmLexer () { }
    AsmLexer (std::string source);
    Token getNextToken (); 
    std::string getCurrLexeme ();
  
  private:
    std::string source;
    std::map<std::string, Token> reserved_word; 
    std::map<char, Token> delim;

    // lexer internal 
    int curr_lexeme_start;
    int curr_lexeme_end;
    int curr_lex_state;
    std::string curr_lexeme;

    // helper functions
    char getNextChar ();
    void lexError (char input);

    bool lexStateStart (char curr_char);
    bool lexStateInt (char curr_char);
    bool lexStateFloat (char curr_char);
    bool lexStateIdent (char curr_char);
    bool lexStateDelim (char curr_char);
};


#endif
