#include "asm_lexer.hpp"
#include <cctype>
#include <iostream>
#include "instr_lookup.hpp"

using std::string;
using std::cout;
using std::endl;


enum LexState {LEX_STATE_START,
               LEX_STATE_INT,
               LEX_STATE_FLOAT,
               LEX_STATE_STRING,
               LEX_STATE_IDENT,
               LEX_STATE_DELIM,
               LEX_STATE_INVALID};


AsmLexer::AsmLexer (string source)
{
  this->source = source;
  curr_lexeme_start = 0;
  curr_lexeme_end = 0;
  curr_lexeme = "";

  reserved_word["setStackSize"] = TOKEN_TYPE_SETSTACKSIZE;
  reserved_word["var"] = TOKEN_TYPE_VAR;
  reserved_word["func"] = TOKEN_TYPE_FUNC;
  reserved_word["param"] = TOKEN_TYPE_PARAM;
  reserved_word["_retVal"] = TOKEN_TYPE_REG_RETVAL;

  delim[':'] = TOKEN_TYPE_COLON;
  delim['['] = TOKEN_TYPE_OPEN_BRACKET;
  delim[']'] = TOKEN_TYPE_CLOSE_BRACKET;
  delim[','] = TOKEN_TYPE_COMMA;
  delim['{'] = TOKEN_TYPE_OPEN_BRACE;
  delim['}'] = TOKEN_TYPE_CLOSE_BRACE;
  delim['\n'] = TOKEN_TYPE_NEWLINE;
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
  curr_lex_state = LEX_STATE_START;

  bool lexeme_done = false;

  // loop until a token is completed.
  char curr_char;       // current character

  while (true)
  {
    curr_char = getNextChar ();
    if (curr_char == '\0')
      break;
    
    switch (curr_lex_state)
    {
      case LEX_STATE_START:
        lexeme_done = lexStateStart (curr_char);
        break;

      case LEX_STATE_INT:
        lexeme_done = lexStateInt (curr_char);
        break;

      case LEX_STATE_FLOAT:
        lexeme_done = lexStateFloat (curr_char);
        break;

      case LEX_STATE_IDENT:
        lexeme_done = lexStateIdent (curr_char);
        break;

      case LEX_STATE_DELIM:
        lexeme_done = lexStateDelim (curr_char);
        break;
    }

    if (!lexeme_done)
      curr_lexeme += curr_char;
    else if (curr_lex_state != LEX_STATE_START)
      break;
  }

  // go back one character since it read one more to determine end of lexeme
  curr_lexeme_end--;

  Token token_type;
  switch (curr_lex_state)
  {
    case LEX_STATE_INT:
      token_type = TOKEN_TYPE_INT;
      break;

    case LEX_STATE_FLOAT:
      token_type = TOKEN_TYPE_FLOAT;
      break;

    case LEX_STATE_IDENT:
      // check if lexeme match with instruction mnemonic or reserved word
      // instruction
      if (InstrLookupTable::isInstr (curr_lexeme))
        token_type = TOKEN_TYPE_INSTR;
      // reserved word
      else if (reserved_word.find (curr_lexeme) != reserved_word.end ())
        token_type = reserved_word[curr_lexeme];
      // identifier
      else
        token_type = TOKEN_TYPE_IDENT;
      break;

    case LEX_STATE_DELIM:
      token_type = delim[curr_lexeme[0]];
      break;
  }
  
  return token_type;
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


bool AsmLexer::lexStateStart (char curr_char)
{
  // flag for adding current character to lexeme
  bool lexeme_done = false;   

  if (isspace (curr_char) && curr_char != '\n')
  {
    curr_lexeme_start++;
    lexeme_done = true;
  }
  else if (isdigit (curr_char))
    curr_lex_state = LEX_STATE_INT;
  else if (curr_char == '.')
    curr_lex_state = LEX_STATE_FLOAT;
  else if (isalpha (curr_char) || curr_char == '_')
    curr_lex_state = LEX_STATE_IDENT;
  else if (delim.find (curr_char) != delim.end ())
    curr_lex_state = LEX_STATE_DELIM;
  else
    lexError (curr_char);

  return lexeme_done;
}

bool AsmLexer::lexStateInt (char curr_char)
{
  // flag for adding current character to lexeme
  bool lexeme_done = false;   

  if (isdigit (curr_char)) { }
  else if (curr_char == '.')
    curr_lex_state = LEX_STATE_FLOAT;

  else if (isspace (curr_char) && curr_char != '\n')
    lexeme_done = true;
  else
    lexError (curr_char);

  return lexeme_done;
}

bool AsmLexer::lexStateFloat (char curr_char)
{
  // flag for adding current character to lexeme
  bool lexeme_done = false;   

  if (isdigit (curr_char)) { }
  else if (isspace (curr_char) && curr_char != '\n')
    lexeme_done = true;
  else
    lexError (curr_char);

  return lexeme_done;
}

bool AsmLexer::lexStateIdent (char curr_char)
{
  // flag for adding current character to lexeme
  bool lexeme_done = false;   

  if (isalpha (curr_char) || isdigit (curr_char) || curr_char == '_') {}
  else if (isspace (curr_char) && curr_char != '\n')
    lexeme_done = true;
  else
    lexError (curr_char);

  return lexeme_done;
}

bool AsmLexer::lexStateDelim (char curr_char)
{
  // lexeme should be done since all delim is one character.
  return true;
}
