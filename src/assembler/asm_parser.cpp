#include "asm_parser.hpp"
#include "error.hpp"

#define GLOBAL_SCOPE 0

using std::string;

AsmParser::AsmParser (string raw_source)
{
  lexer = AsmLexer (raw_source);
}

void AsmParser::readToken (Token req_token)
{
  if (lexer.getNextToken () != req_token)
  {
    string msg = token2string (req_token) + " expected";
    exitOnCodeError (msg, lexer); 
  }
}

void AsmParser::parse ()
{
  curr_scope = GLOBAL_SCOPE; 

  while (true)
  {
    if (lexer.peekNextToken () == TOKEN_TYPE_EOF)
      return;
    else
      parseLine ();
  }
}

void AsmParser::parseLine ()
{
  Token first_token = lexer.getNextToken ();
  switch (first_token)
  {
    case TOKEN_TYPE_NEWLINE:
      return;

    case TOKEN_TYPE_FUNC:
      parseFunc ();
      break;

    default:
      exitOnCodeError ("Unexpected token", lexer);
  }
}

void AsmParser::parseFunc ()
{
  if (curr_scope != GLOBAL_SCOPE)
    exitOnCodeError ("Function cannot be defined inside another function", lexer);

  // function name
  readToken (TOKEN_TYPE_IDENT);

  // add function to the function table and check for redefinition
  int func_index = func_table.addFunc (lexer.getCurrLexeme (), lexer.getCurrLineIndex ()); 
  if (func_index == -1)
    exitOnCodeError ("Function was already defined somewhere else", lexer);

  // change scope to the function
  curr_scope = func_index;

  if (lexer.peekNextToken () == TOKEN_TYPE_NEWLINE)
    readToken (TOKEN_TYPE_NEWLINE);

  // start the code block with opening curly brace.
  readToken (TOKEN_TYPE_OPEN_BRACE);

  // parse function body
  parseBlock ();

  // once block have been parsed, return to global scope
  curr_scope = GLOBAL_SCOPE;
}

void AsmParser::parseBlock ()
{
  if (curr_scope == GLOBAL_SCOPE)
    exitOnCodeError ("Code block can only be used for defining function scope", lexer);

  while (lexer.peekNextToken () != TOKEN_TYPE_CLOSE_BRACE)
    parseLine ();

  // end the code block with closing curly brace.
  readToken (TOKEN_TYPE_CLOSE_BRACE);
}
