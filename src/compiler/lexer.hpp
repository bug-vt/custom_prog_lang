#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include "token.hpp"
#include "preprocess.hpp"

class Lexer
{
  public:
    Lexer () { }
    Lexer (std::string raw_source);
    Token getNextToken (); 
    std::string getCurrLexeme ();
    Token peekNextToken ();
    bool goToNextLine ();
    void reset ();
    void error (std::string msg);
  
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

    // reserved words, delimiters, operators
    std::unordered_map<std::string, Token> reserved_word; 
    std::unordered_map<char, Token> delim;
    std::unordered_map<std::string, Token> op;

    // lexer internal 
    Lexeme curr_lexeme;
    Lexeme prev_lexeme;
    int curr_lex_state;
    bool add_curr_char;
    bool lexeme_done;

    // helper functions
    char getNextChar ();
    void copyLexeme (Lexeme &dest, Lexeme &source);
    void undoGetNextToken ();
    bool isOpChar (char curr_char);

    // state machine 
    std::unordered_map<int, void (Lexer::*)(char)> state_machine;
    // lexer states
    void lexStateStart (char curr_char);
    void lexStateInt (char curr_char);
    void lexStateFloat (char curr_char);
    void lexStateIdent (char curr_char);
    void lexStateOp (char curr_char);
    void lexStateDelim (char curr_char);
    void lexStateString (char curr_char);
    void lexStateStringEscape (char curr_char);
    void lexStateCloseQuote (char curr_char);
    void lexStateInvalid (char curr_char);
};


#endif
