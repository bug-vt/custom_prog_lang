#include "parser.hpp"

#define GLOBAL_SCOPE 0

using std::string;

Parser::Parser (string raw_source)
{
  lexer = Lexer (raw_source);
}

void Parser::readToken (Token req_token)
{
  if (lexer.getNextToken () != req_token)
  {
    string msg = token2string (req_token) + " expected";
    lexer.error (msg); 
  }
}

void Parser::parse ()
{
  lexer.reset ();
  curr_scope = GLOBAL_SCOPE; 

  while (true)
  {
    // end of file is reached. End the parsing
    if (lexer.peekNextToken () == TOKEN_TYPE_EOF)
      break;
    // otherwise, go to next statement
    else
      parseStatement ();
  }
}

void Parser::parseStatement ()
{
  Token first_token = lexer.getNextToken ();
  switch (first_token)
  {
    case TOKEN_TYPE_SEMICOLON:
      break;

    case TOKEN_TYPE_EOF:
      lexer.error ("Unexpected end of file");
      break;

    case TOKEN_TYPE_OPEN_BRACE:
      parseBlock ();
      break;

    default:
      lexer.error ("Unexpected token");
  }
}


void Parser::parseBlock ()
{
  while (lexer.peekNextToken () != TOKEN_TYPE_CLOSE_BRACE)
    parseStatement ();

  // end the code block with closing curly brace.
  readToken (TOKEN_TYPE_CLOSE_BRACE);
}

