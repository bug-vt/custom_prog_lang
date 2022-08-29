#include "asm_lexer.hpp"
#include <cctype>
#include <iostream>

using std::string;
using std::cout;
using std::endl;


enum LexState {LEX_STATE_START,
               LEX_STATE_INT,
               LEX_STATE_FLOAT};


AsmLexer::AsmLexer (string source)
{
  this->source = source;
  curr_lexeme_start = 0;
  curr_lexeme_end = 0;
  curr_lexeme = "";

  token_type[LEX_STATE_START] = TOKEN_TYPE_EOF;
  token_type[LEX_STATE_INT] =   TOKEN_TYPE_INT;
  token_type[LEX_STATE_FLOAT] = TOKEN_TYPE_FLOAT;
}

string AsmLexer::getCurrLexeme ()
{
  return curr_lexeme;
}

Token AsmLexer::getNextToken ()
{
  // start the new lexeme at the end of the last one
  curr_lexeme_start = curr_lexeme_end;
 
  // check if reached end of the character stream
  if (curr_lexeme_start >= source.length ())
    return TOKEN_TYPE_EOF;

  // initialize current lexeme and state
  curr_lexeme = "";
  int curr_lex_state = LEX_STATE_START;

  bool lexeme_done = false;

  // loop until a token is completed.
  char curr_char;       // current character
  bool add_curr_char;   // flag for adding current character to lexeme

  while (true)
  {
    curr_char = getNextChar ();
    if (curr_char == '\0')
      break;
    
    add_curr_char = true;
    switch (curr_lex_state)
    {
      case LEX_STATE_START:
        if (isspace (curr_char) && curr_char != '\n')
        {
          curr_lexeme_start++;
          add_curr_char = false;
        }

        else if (isdigit (curr_char))
          curr_lex_state = LEX_STATE_INT;

        else if (curr_char == '.')
          curr_lex_state = LEX_STATE_FLOAT;

        else
          lexError (curr_char);
        break;

      case LEX_STATE_INT:
        if (isdigit (curr_char)) { }
        else if (curr_char == '.')
          curr_lex_state = LEX_STATE_FLOAT;

        else if (isspace (curr_char) && curr_char != '\n')
        {
          add_curr_char = false;
          lexeme_done = true;
        }
        else
          lexError (curr_char);
        break;

      case LEX_STATE_FLOAT:
        if (isdigit (curr_char)) { }
        else if (isspace (curr_char) && curr_char != '\n')
        {
          add_curr_char = false;
          lexeme_done = true;
        }
        else
          lexError (curr_char);
        break;
    }

    if (add_curr_char)
      curr_lexeme += curr_char;

    if (lexeme_done)
      break;
  }

  // go back one character since it read one more to determine end of lexeme
  curr_lexeme_end--;
  
  return token_type[curr_lex_state];
}

char AsmLexer::getNextChar ()
{
  return source[curr_lexeme_end++];
}

void AsmLexer::lexError (char input)
{
  cout << "Error: unexpected character '" << input << "' found." << endl;
  exit (-1);
}
