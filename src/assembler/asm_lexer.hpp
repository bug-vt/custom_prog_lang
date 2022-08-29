#ifndef _ASM_LEXER
#define _ASM_LEXER

#include <string>
#include <map>

typedef int Token;
enum TokenType {TOKEN_TYPE_EOF,
                TOKEN_TYPE_INT,
                TOKEN_TYPE_FLOAT};

class AsmLexer
{
  public:
    AsmLexer () { }
    AsmLexer (std::string source);
    Token getNextToken (); 
    std::string getCurrLexeme ();
  
  private:
    std::string source;
    std::map<int, Token> token_type; 

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
};


#endif
