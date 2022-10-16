#include "lexer.hpp"
#include <cctype>
#include <iostream>
#include <sstream>

using std::string;
using std::cout;
using std::endl;
using std::istringstream;


enum LexState {LEX_STATE_START,
               LEX_STATE_NEGATIVE,
               LEX_STATE_INT,
               LEX_STATE_FLOAT,
               LEX_STATE_STRING,
               LEX_STATE_STRING_ESCAPE,
               LEX_STATE_CLOSE_QUOTE,
               LEX_STATE_IDENT,
               LEX_STATE_DELIM,
               LEX_STATE_COMMENT,
               LEX_STATE_LINE_COMMENT,
               LEX_STATE_BLOCK_COMMENT,
               LEX_STATE_BLOCK_COMMENT_END,
               LEX_STATE_INVALID};


Lexer::Lexer (string raw_source)
{
  // store given source code line by line
  // this help output the line that cause error
  istringstream code_stream (raw_source);
  string line;
  while (getline (code_stream, line))
  {
    line += '\n';
    source_code.push_back (line);
  }

  // initialize lexeme
  curr_lexeme.lexeme_start = 0;
  curr_lexeme.lexeme_end = 0;
  curr_lexeme.lexeme = "";
  if (source_code.size () > 0)
    curr_lexeme.line = source_code[0];
  curr_lexeme.line_index = 0;

  // list of reserved words
  reserved_word["var"] = TOKEN_TYPE_VAR;
  reserved_word["true"] = TOKEN_TYPE_TRUE;
  reserved_word["false"] = TOKEN_TYPE_FALSE;
  reserved_word["if"] = TOKEN_TYPE_IF;
  reserved_word["else"] = TOKEN_TYPE_ELSE;
  reserved_word["break"] = TOKEN_TYPE_BREAK;
  reserved_word["continue"] = TOKEN_TYPE_CONTINUE;
  reserved_word["for"] = TOKEN_TYPE_FOR;
  reserved_word["while"] = TOKEN_TYPE_WHILE;
  reserved_word["func"] = TOKEN_TYPE_FUNC;
  reserved_word["return"] = TOKEN_TYPE_RETURN;

  // list of delimiters
  delim['['] = TOKEN_TYPE_OPEN_BRACKET;
  delim[']'] = TOKEN_TYPE_CLOSE_BRACKET;
  delim[','] = TOKEN_TYPE_COMMA;
  delim['{'] = TOKEN_TYPE_OPEN_BRACE;
  delim['}'] = TOKEN_TYPE_CLOSE_BRACE;
  delim['('] = TOKEN_TYPE_OPEN_PAREN;
  delim[')'] = TOKEN_TYPE_CLOSE_PAREN;
  delim[';'] = TOKEN_TYPE_SEMICOLON;

  // initialize state machine
  state_machine[LEX_STATE_START] = &Lexer::lexStateStart; 
  state_machine[LEX_STATE_NEGATIVE] = &Lexer::lexStateNegative; 
  state_machine[LEX_STATE_INT] = &Lexer::lexStateInt; 
  state_machine[LEX_STATE_FLOAT] = &Lexer::lexStateFloat; 
  state_machine[LEX_STATE_IDENT] = &Lexer::lexStateIdent; 
  state_machine[LEX_STATE_DELIM] = &Lexer::lexStateDelim; 
  state_machine[LEX_STATE_STRING] = &Lexer::lexStateString; 
  state_machine[LEX_STATE_STRING_ESCAPE] = &Lexer::lexStateStringEscape; 
  state_machine[LEX_STATE_CLOSE_QUOTE] = &Lexer::lexStateCloseQuote; 
  state_machine[LEX_STATE_COMMENT] = &Lexer::lexStateComment; 
  state_machine[LEX_STATE_LINE_COMMENT] = &Lexer::lexStateLineComment; 
  state_machine[LEX_STATE_BLOCK_COMMENT] = &Lexer::lexStateBlockComment; 
  state_machine[LEX_STATE_BLOCK_COMMENT_END] = &Lexer::lexStateBlockCommentEnd; 
  state_machine[LEX_STATE_INVALID] = &Lexer::lexStateInvalid; 
}

string Lexer::getCurrLexeme ()
{
  return curr_lexeme.lexeme;
}

Token Lexer::getNextToken ()
{
  // save the current lexeme for future rewinding
  copyLexeme (prev_lexeme, curr_lexeme);

  // start the new lexeme at the end of the last one
  curr_lexeme.lexeme_start = curr_lexeme.lexeme_end;

  // initialize current lexeme and state
  curr_lexeme.lexeme = "";
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
    
    // invoke appropriate function from table, index by current state number 
    (*this.*state_machine.at (curr_lex_state)) (curr_char);

    if (add_curr_char)
      curr_lexeme.lexeme += curr_char;

    if (lexeme_done)
      break;
  }

  // go back one character since it read one more to determine end of lexeme
  curr_lexeme.lexeme_end--;

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
      // reserved word
      if (reserved_word.find (curr_lexeme.lexeme) != reserved_word.end ())
        token_type = reserved_word[curr_lexeme.lexeme];
      // identifier
      else
        token_type = TOKEN_TYPE_IDENT;
      break;

    case LEX_STATE_DELIM:
      token_type = delim[curr_lexeme.lexeme[0]];
      break;

    case LEX_STATE_CLOSE_QUOTE:
      token_type = TOKEN_TYPE_STRING;
      break;

    default:
      token_type = TOKEN_TYPE_INVALID;
  }
  
  return token_type;
}

Token Lexer::peekNextToken ()
{
  // temporary save current lexeme
  Lexeme tmp_lexeme;
  copyLexeme (tmp_lexeme, curr_lexeme);

  // read next lexeme/token
  Token next_token = getNextToken ();
 
  // restore back to current lexeme
  copyLexeme (curr_lexeme, tmp_lexeme);
  return next_token;
}


void Lexer::undoGetNextToken ()
{
  copyLexeme (curr_lexeme, prev_lexeme);
}


char Lexer::getNextChar ()
{
  // check if lexer reach end of source file
  if (curr_lexeme.line_index >= source_code.size ())
    return '\0';

  // otherwise, when current lexeme end index go beyond of current line,
  if (curr_lexeme.lexeme_end >= curr_lexeme.line.length ())
  {
    if (!goToNextLine ())
    {
      curr_lexeme.line = "";
      return '\0';
    }
  }

  return curr_lexeme.line[curr_lexeme.lexeme_end++];
}

bool Lexer::goToNextLine ()
{
  curr_lexeme.line_index++;
  // make sure lexer did not reach end of source file,
  if (curr_lexeme.line_index >= source_code.size ())
    return false;

  // move to next line.
  curr_lexeme.line = source_code[curr_lexeme.line_index];
  curr_lexeme.lexeme_start = 0;
  curr_lexeme.lexeme_end = 0;

  return true;
}


void Lexer::reset ()
{
  // rewind to beginning of the first source code line
  curr_lexeme.line_index = 0;
  if (source_code.size () > 0)
    curr_lexeme.line = source_code[0];
  curr_lexeme.lexeme_start = 0;
  curr_lexeme.lexeme_end = 0;
  curr_lexeme.lexeme = "";
  
  curr_lex_state = LEX_STATE_INVALID;
}


void Lexer::copyLexeme (Lexeme &dest, Lexeme &source)
{
  dest.lexeme = source.lexeme;
  dest.lexeme_start = source.lexeme_start;
  dest.lexeme_end = source.lexeme_end;
  dest.line = source.line;
  dest.line_index = source.line_index;
}


string Lexer::getCurrLine ()
{
  return curr_lexeme.line;
}

int Lexer::getCurrLineIndex ()
{
  return curr_lexeme.line_index;
}

int Lexer::getLexemeStartIndex ()
{
  return curr_lexeme.lexeme_start;
}


// ---------------------------------------------------------------------
// Lexer States

void Lexer::lexStateStart (char curr_char)
{
  // white space is read
  if (isspace (curr_char))
  {
    curr_lexeme.lexeme_start++;
    add_curr_char = false;
  }
  else if (curr_char == '-')
    curr_lex_state = LEX_STATE_NEGATIVE;
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
  else if (curr_char == '/')
  {
    curr_lex_state = LEX_STATE_COMMENT;
    add_curr_char = false;
  }
  // invalid character is read 
  else
    curr_lex_state = LEX_STATE_INVALID;
}

void Lexer::lexStateNegative (char curr_char)
{
  if (isdigit (curr_char))
    curr_lex_state = LEX_STATE_INT;
  else if (curr_char == '.')
    curr_lex_state = LEX_STATE_FLOAT;
  // invalid character is read 
  else
  {
    // white space or delimiter is read
    if (isspace (curr_char) || delim.find (curr_char) != delim.end ())
    {
      add_curr_char = false;
      lexeme_done = true;
    }
    curr_lex_state = LEX_STATE_INVALID;
  }
}

void Lexer::lexStateInt (char curr_char)
{
  if (isdigit (curr_char)) { }
  else if (curr_char == '.')
    curr_lex_state = LEX_STATE_FLOAT;
  // white space or delimiter is read
  else if (isspace (curr_char)
           || delim.find (curr_char) != delim.end ())
  {
    add_curr_char = false;
    lexeme_done = true;
  }
  // invalid character is read 
  else
    curr_lex_state = LEX_STATE_INVALID;
}

void Lexer::lexStateFloat (char curr_char)
{
  if (isdigit (curr_char)) { }
  // white space or delimiter is read
  else if (isspace (curr_char)
           || delim.find (curr_char) != delim.end ())
  {
    add_curr_char = false;
    lexeme_done = true;
  }
  // invalid character is read 
  else
    curr_lex_state = LEX_STATE_INVALID;
}

void Lexer::lexStateIdent (char curr_char)
{
  if (isalpha (curr_char) || isdigit (curr_char) || curr_char == '_') {}
  // white space or delimiter is read
  else if (isspace (curr_char)
           || delim.find (curr_char) != delim.end ())
  {
    add_curr_char = false;
    lexeme_done = true;
  }
  // invalid character is read 
  else
    curr_lex_state = LEX_STATE_INVALID;
}

void Lexer::lexStateDelim (char curr_char)
{
  // lexeme should be done since all delim is one character.
  add_curr_char = false;
  lexeme_done = true;
}

void Lexer::lexStateString (char curr_char)
{
  if (curr_char == '"')
  {
    add_curr_char = false;
    curr_lex_state = LEX_STATE_CLOSE_QUOTE;
  }
  else if (curr_char == '\\')
  {
    add_curr_char = false;
    curr_lex_state = LEX_STATE_STRING_ESCAPE;
  }
}

void Lexer::lexStateStringEscape (char curr_char)
{
  if (curr_char == 'n')
  {
    add_curr_char = false;
    curr_lexeme.lexeme += "\n";
  }
  else if (curr_char == 't')
  {
    add_curr_char = false;
    curr_lexeme.lexeme += "\t";
  }
  // for other escape characters such as " (from \")
  // the current character can be simply added.

  // switch back to the string state.
  curr_lex_state = LEX_STATE_STRING;
}

void Lexer::lexStateCloseQuote (char curr_char)
{
  // At this point, string lexeme is done.
  add_curr_char = false;
  lexeme_done = true;
}

void Lexer::lexStateComment (char curr_char)
{
  add_curr_char = false;

  if (curr_char == '/')
    curr_lex_state = LEX_STATE_LINE_COMMENT;
  else if (curr_char == '*')
    curr_lex_state = LEX_STATE_BLOCK_COMMENT;
  // invalid character is read 
  else
    curr_lex_state = LEX_STATE_INVALID;
}

void Lexer::lexStateLineComment (char curr_char)
{
  add_curr_char = false;

  if (curr_char == '\n')
    curr_lex_state = LEX_STATE_START;
}

void Lexer::lexStateBlockComment (char curr_char)
{
  add_curr_char = false;
  
  if (curr_char == '*')
    curr_lex_state = LEX_STATE_BLOCK_COMMENT_END;
}

void Lexer::lexStateBlockCommentEnd (char curr_char)
{
  add_curr_char = false;

  if (curr_char == '/')
    curr_lex_state = LEX_STATE_START;
  else
    curr_lex_state = LEX_STATE_BLOCK_COMMENT;
}

void Lexer::lexStateInvalid (char curr_char)
{
  // white space or delimiter is read
  if (isspace (curr_char) || delim.find (curr_char) != delim.end ())
  {
    add_curr_char = false;
    lexeme_done = true;
  }
}
