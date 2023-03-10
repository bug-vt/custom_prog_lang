#include "lexer.hpp"
#include <cctype>
#include <iostream>
#include <sstream>
#include <algorithm>

using std::string;
using std::cout;
using std::endl;
using std::istringstream;


enum LexState {LEX_STATE_START,
               LEX_STATE_INT,
               LEX_STATE_FLOAT,
               LEX_STATE_IDENT,
               LEX_STATE_OP,
               LEX_STATE_DELIM,
               LEX_STATE_STRING,
               LEX_STATE_STRING_ESCAPE,
               LEX_STATE_CLOSE_QUOTE,
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

  // remove comments
  Preprocess::removeComments (source_code);

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
  reserved_word["print"] = TOKEN_TYPE_PRINT;

  // list of delimiters
  delim['['] = TOKEN_TYPE_OPEN_BRACKET;
  delim[']'] = TOKEN_TYPE_CLOSE_BRACKET;
  delim[','] = TOKEN_TYPE_COMMA;
  delim['{'] = TOKEN_TYPE_OPEN_BRACE;
  delim['}'] = TOKEN_TYPE_CLOSE_BRACE;
  delim['('] = TOKEN_TYPE_OPEN_PAREN;
  delim[')'] = TOKEN_TYPE_CLOSE_PAREN;
  delim[';'] = TOKEN_TYPE_SEMICOLON;

  // list of operators
  // arithmetic
  op["="] = TOKEN_TYPE_ASSIGN;
  op["+"] = TOKEN_TYPE_ADD;
  op["-"] = TOKEN_TYPE_SUB;
  op["*"] = TOKEN_TYPE_MUL;
  op["/"] = TOKEN_TYPE_DIV;
  op["%"] = TOKEN_TYPE_MOD;
  op["^"] = TOKEN_TYPE_EXP;
  op["++"] = TOKEN_TYPE_INC;
  op["--"] = TOKEN_TYPE_DEC;
  op["+="] = TOKEN_TYPE_ASSIGN_ADD;
  op["-="] = TOKEN_TYPE_ASSIGN_SUB;
  op["*="] = TOKEN_TYPE_ASSIGN_MUL;
  op["/="] = TOKEN_TYPE_ASSIGN_DIV;
  op["%="] = TOKEN_TYPE_ASSIGN_MOD;
  op["^="] = TOKEN_TYPE_ASSIGN_EXP;
  // bitwise
  op["&"] = TOKEN_TYPE_BITWISE_AND;
  op["|"] = TOKEN_TYPE_BITWISE_OR;
  op["#"] = TOKEN_TYPE_BITWISE_XOR;
  op["~"] = TOKEN_TYPE_BITWISE_NOT;
  op["<<"] = TOKEN_TYPE_BITWISE_SHIFT_LEFT;
  op[">>"] = TOKEN_TYPE_BITWISE_SHIFT_RIGHT;
  op["&="] = TOKEN_TYPE_ASSIGN_AND;
  op["|="] = TOKEN_TYPE_ASSIGN_OR;
  op["#="] = TOKEN_TYPE_ASSIGN_XOR;
  op["<<="] = TOKEN_TYPE_ASSIGN_SHIFT_LEFT;
  op[">>="] = TOKEN_TYPE_ASSIGN_SHIFT_RIGHT;
  // logical
  op["&&"] = TOKEN_TYPE_LOGICAL_AND;
  op["||"] = TOKEN_TYPE_LOGICAL_OR;
  op["!"] = TOKEN_TYPE_LOGICAL_NOT;
  // relational
  op["=="] = TOKEN_TYPE_EQUAL;
  op["!="] = TOKEN_TYPE_NOT_EQUAL;
  op["<"] = TOKEN_TYPE_LESS;
  op[">"] = TOKEN_TYPE_GREATER;
  op["<="] = TOKEN_TYPE_LESS_EQUAL;
  op[">="] = TOKEN_TYPE_GREATER_EQUAL;

  // initialize state machine
  state_machine[LEX_STATE_START] = &Lexer::lexStateStart; 
  state_machine[LEX_STATE_INT] = &Lexer::lexStateInt; 
  state_machine[LEX_STATE_FLOAT] = &Lexer::lexStateFloat; 
  state_machine[LEX_STATE_IDENT] = &Lexer::lexStateIdent; 
  state_machine[LEX_STATE_OP] = &Lexer::lexStateOp; 
  state_machine[LEX_STATE_DELIM] = &Lexer::lexStateDelim; 
  state_machine[LEX_STATE_STRING] = &Lexer::lexStateString; 
  state_machine[LEX_STATE_STRING_ESCAPE] = &Lexer::lexStateStringEscape; 
  state_machine[LEX_STATE_CLOSE_QUOTE] = &Lexer::lexStateCloseQuote; 
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
  TokenType token_type = TOKEN_TYPE_INVALID;
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

    case LEX_STATE_OP:
      // find matching operator 
      if (op.find (curr_lexeme.lexeme) != op.end ())
        token_type = op[curr_lexeme.lexeme];
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
  
  return Token (token_type, curr_lexeme.lexeme);
}

TokenType Lexer::peekNextToken ()
{
  // temporary save current lexeme
  Lexeme tmp_lexeme;
  copyLexeme (tmp_lexeme, curr_lexeme);

  // read next lexeme/token
  Token next_token = getNextToken ();
 
  // restore back to current lexeme
  copyLexeme (curr_lexeme, tmp_lexeme);
  return next_token.type;
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

void Lexer::error (string msg)
{
  cout << "Error: " << msg << endl;
  cout << "Line: " << curr_lexeme.line_index << endl;

  // replace all tabs to white space
  string line = curr_lexeme.line;
  replace (line.begin (), line.end (), '\t', ' '); 

  // show error line and caret at the start of the error lexeme
  cout << line;
  cout << string (curr_lexeme.lexeme_start, ' ') << '^' << endl;

  cout << "Fail to compile\n" << endl;
  exit (EXIT_FAILURE);
}


void Lexer::copyLexeme (Lexeme &dest, Lexeme &source)
{
  dest.lexeme = source.lexeme;
  dest.lexeme_start = source.lexeme_start;
  dest.lexeme_end = source.lexeme_end;
  dest.line = source.line;
  dest.line_index = source.line_index;
}


bool Lexer::isOpChar (char curr_char)
{
  if (curr_char == '+' || curr_char == '-' || curr_char == '*' 
      || curr_char == '/' || curr_char == '^' || curr_char == '%'
      || curr_char == '&' || curr_char == '|' || curr_char == '#'
      || curr_char == '~' || curr_char == '!' || curr_char == '=' 
      || curr_char == '<' || curr_char == '>')
    return true;

  return false;
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
  else if (isdigit (curr_char))
    curr_lex_state = LEX_STATE_INT;
  else if (curr_char == '.')
    curr_lex_state = LEX_STATE_FLOAT;
  else if (isalpha (curr_char) || curr_char == '_')
    curr_lex_state = LEX_STATE_IDENT;
  else if (isOpChar (curr_char))
    curr_lex_state = LEX_STATE_OP;
  else if (delim.find (curr_char) != delim.end ())
    curr_lex_state = LEX_STATE_DELIM;
  else if (curr_char == '"')
  {
    curr_lex_state = LEX_STATE_STRING;
    //add_curr_char = false;
  }
  // invalid character is read 
  else
    curr_lex_state = LEX_STATE_INVALID;
}

void Lexer::lexStateInt (char curr_char)
{
  if (isdigit (curr_char)) { }
  else if (curr_char == '.')
    curr_lex_state = LEX_STATE_FLOAT;
  // white space, delimiter, or operator is read
  else if (isspace (curr_char)
           || delim.find (curr_char) != delim.end ()
           || isOpChar (curr_char))
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
  // white space, delimiter, or operator is read
  else if (isspace (curr_char)
           || delim.find (curr_char) != delim.end ()
           || isOpChar (curr_char))
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
  // white space, delimiter, or operator is read
  else if (isspace (curr_char)
           || delim.find (curr_char) != delim.end ()
           || isOpChar (curr_char))
  {
    add_curr_char = false;
    lexeme_done = true;
  }
  // invalid character is read 
  else
    curr_lex_state = LEX_STATE_INVALID;
}

void Lexer::lexStateOp (char curr_char)
{
  if (isOpChar (curr_char)) {} 
  // white space, delimiter, number, alpha, or etc is read
  else 
  {
    add_curr_char = false;
    lexeme_done = true;
  }
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
    //add_curr_char = false;
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

void Lexer::lexStateInvalid (char curr_char)
{
  // white space or delimiter is read
  if (isspace (curr_char) || delim.find (curr_char) != delim.end ())
  {
    add_curr_char = false;
    lexeme_done = true;
  }
}
