#ifndef _ASM_LEXER
#define _ASM_LEXER

#include <string>
#include <map>
#include <vector>
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
    AsmLexer (std::string raw_source);
    Token getNextToken (); 
    std::string getCurrLexeme ();
    void rewindTokenStream ();
  
  private:
    struct Lexeme
    {
      std::string line;
      int line_index;
      int lexeme_start;
      int lexeme_end;
      std::string lexeme;
    };

    // source code
    std::vector<std::string> source_code;

    // reserved words/delimiters
    std::map<std::string, Token> reserved_word; 
    std::map<char, Token> delim;


    // lexer internal 
    Lexeme curr_lexeme;
    Lexeme prev_lexeme;
    int curr_lex_state;
    bool add_curr_char;
    bool lexeme_done;

    // helper functions
    char getNextChar ();
    void copyLexeme (Lexeme &dest, Lexeme &source);
    void lexError (char input);

    // lexer states
    void lexStateStart (char curr_char);
    void lexStateInt (char curr_char);
    void lexStateFloat (char curr_char);
    void lexStateIdent (char curr_char);
    void lexStateDelim (char curr_char);
    void lexStateString (char curr_char);
    void lexStateStringEscape (char curr_char);
    void lexStateCloseQuote (char curr_char);
    void lexStateComment (char curr_char);
    void lexStateInvalid (char curr_char);
};


#endif
