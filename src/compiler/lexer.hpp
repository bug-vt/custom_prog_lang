#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include "token.hpp"


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
    // used for displaying error
    std::string getCurrLine ();
    int getCurrLineIndex ();
    int getLexemeStartIndex ();
  
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
    void undoGetNextToken ();

    // state machine 
    std::unordered_map<int, void (Lexer::*)(char)> state_machine;
    // lexer states
    void lexStateStart (char curr_char);
    void lexStateNegative (char curr_char);
    void lexStateInt (char curr_char);
    void lexStateFloat (char curr_char);
    void lexStateIdent (char curr_char);
    void lexStateDelim (char curr_char);
    void lexStateString (char curr_char);
    void lexStateStringEscape (char curr_char);
    void lexStateCloseQuote (char curr_char);
    void lexStateComment (char curr_char);
    void lexStateLineComment (char curr_char);
    void lexStateBlockComment (char curr_char);
    void lexStateBlockCommentEnd (char curr_char);
    void lexStateInvalid (char curr_char);
};


#endif
