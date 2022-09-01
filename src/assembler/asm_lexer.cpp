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
               LEX_STATE_STRING_ESCAPE,
               LEX_STATE_CLOSE_QUOTE,
               LEX_STATE_IDENT,
               LEX_STATE_DELIM,
               LEX_STATE_COMMENT,
               LEX_STATE_INVALID};


AsmLexer::AsmLexer (string source)
{
  this->source = source;
  curr_lexeme_start = 0;
  curr_lexeme_end = 0;
  curr_lexeme = "";

  // list of reserved words
  reserved_word["setStackSize"] = TOKEN_TYPE_SETSTACKSIZE;
  reserved_word["var"] = TOKEN_TYPE_VAR;
  reserved_word["func"] = TOKEN_TYPE_FUNC;
  reserved_word["param"] = TOKEN_TYPE_PARAM;
  reserved_word["_retVal"] = TOKEN_TYPE_REG_RETVAL;

  // list of delimiters
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
  lexeme_done = false;

  // loop until a token is completed.
  char curr_char;       // current character

  while (true)
  {
    curr_char = getNextChar ();
    if (curr_char == '\0')
      break;

    add_curr_char = true;
    
    switch (curr_lex_state)
    {
      case LEX_STATE_START:
        lexStateStart (curr_char);
        break;

      case LEX_STATE_INT:
        lexStateInt (curr_char);
        break;

      case LEX_STATE_FLOAT:
        lexStateFloat (curr_char);
        break;

      case LEX_STATE_IDENT:
        lexStateIdent (curr_char);
        break;

      case LEX_STATE_DELIM:
        lexStateDelim (curr_char);
        break;

      case LEX_STATE_STRING:
        lexStateString (curr_char);
        break;

      case LEX_STATE_STRING_ESCAPE:
        lexStateStringEscape (curr_char);
        break;

      case LEX_STATE_CLOSE_QUOTE:
        lexStateCloseQuote (curr_char);
        break;

      case LEX_STATE_COMMENT:
        lexStateComment (curr_char);
        break;
    }

    if (add_curr_char)
      curr_lexeme += curr_char;

    if (lexeme_done)
      break;
  }

  // go back one character since it read one more to determine end of lexeme
  curr_lexeme_end--;

  // determine token type from lexing state.
  Token token_type;
  switch (curr_lex_state)
  {
    case LEX_STATE_START:
      token_type = TOKEN_TYPE_EOF;
      break;

    case LEX_STATE_INT:
      token_type = TOKEN_TYPE_INT;
      break;

    case LEX_STATE_FLOAT:
      token_type = TOKEN_TYPE_FLOAT;
      break;

    case LEX_STATE_IDENT:
      // check if lexeme match with instruction mnemonic or reserved word.
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

    case LEX_STATE_CLOSE_QUOTE:
      token_type = TOKEN_TYPE_STRING;
      break;

    default:
      token_type = TOKEN_TYPE_INVALID;
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

// ---------------------------------------------------------------------
// Lexer States

void AsmLexer::lexStateStart (char curr_char)
{
  if (isspace (curr_char) && curr_char != '\n')
  {
    curr_lexeme_start++;
    add_curr_char = false;
  }
  else if (isdigit (curr_char))
    curr_lex_state = LEX_STATE_INT;
  else if (curr_char == '.')
    curr_lex_state = LEX_STATE_FLOAT;
  else if (isalpha (curr_char) || curr_char == '_')
    curr_lex_state = LEX_STATE_IDENT;
  else if (delim.find (curr_char) != delim.end ())
    curr_lex_state = LEX_STATE_DELIM;
  else if (curr_char == '"')
  {
    curr_lex_state = LEX_STATE_STRING;
    add_curr_char = false;
  }
  else if (curr_char == ';')
  {
    curr_lex_state = LEX_STATE_COMMENT;
    add_curr_char = false;
  }
  else
    lexError (curr_char);
}

void AsmLexer::lexStateInt (char curr_char)
{
  if (isdigit (curr_char)) { }
  else if (curr_char == '.')
    curr_lex_state = LEX_STATE_FLOAT;

  else if (isspace (curr_char))
  {
    add_curr_char = false;
    lexeme_done = true;
  }
  else
    lexError (curr_char);
}

void AsmLexer::lexStateFloat (char curr_char)
{
  if (isdigit (curr_char)) { }
  else if (isspace (curr_char))
  {
    add_curr_char = false;
    lexeme_done = true;
  }
  else
    lexError (curr_char);
}

void AsmLexer::lexStateIdent (char curr_char)
{
  if (isalpha (curr_char) || isdigit (curr_char) || curr_char == '_') {}
  else if (isspace (curr_char))
  {
    add_curr_char = false;
    lexeme_done = true;
  }
  else
    lexError (curr_char);
}

void AsmLexer::lexStateDelim (char curr_char)
{
  // lexeme should be done since all delim is one character.
  add_curr_char = false;
  lexeme_done = true;
}

void AsmLexer::lexStateString (char curr_char)
{
  if (curr_char == '"')
  {
    add_curr_char = false;
    curr_lex_state = LEX_STATE_CLOSE_QUOTE;
  }
  else if (curr_char == '\\')
  {
    curr_lex_state = LEX_STATE_STRING_ESCAPE;
    add_curr_char = false;
  }
}

void AsmLexer::lexStateStringEscape (char curr_char)
{
  // This let the escape character to be added,
  // then switch back to the string state.
  curr_lex_state = LEX_STATE_STRING;
}

void AsmLexer::lexStateCloseQuote (char curr_char)
{
  // At this point, string lexeme is done.
  add_curr_char = false;
  lexeme_done = true;
}

void AsmLexer::lexStateComment (char curr_char)
{
  add_curr_char = false;

  if (curr_char == '\n')
    curr_lex_state = LEX_STATE_START;
}
