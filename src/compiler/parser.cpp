#include "parser.hpp"
#include <vector>

#define GLOBAL_SCOPE 0

using std::string;
using std::vector;

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

    case TOKEN_TYPE_FUNC:
      parseFunc ();
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

void Parser::parseFunc ()
{
  if (curr_scope != GLOBAL_SCOPE)
    lexer.error ("Function cannot be defined inside another function");

  // function name
  readToken (TOKEN_TYPE_IDENT);
  string func_name = lexer.getCurrLexeme ();
  // add function to the function table and check for redefinition
  int func_index = func_table.addFunc (func_name); 
  if (func_index == -1)
    lexer.error ("Function was already defined somewhere else");

  // change scope to the function
  curr_scope = func_index;

  readToken (TOKEN_TYPE_OPEN_PAREN);
  // Check if there are parameters
  if (lexer.peekNextToken () != TOKEN_TYPE_CLOSE_PAREN)
  {
    vector<Symbol> param_list;
    
    // read parameters
    while (true)
    {
      // add parameters to local parameter list
      readToken (TOKEN_TYPE_IDENT);
      Symbol param (lexer.getCurrLexeme (), curr_scope);  
      param_list.push_back (param);

      // check if the parser reached end of parameters
      if (lexer.peekNextToken () == TOKEN_TYPE_CLOSE_PAREN)
        break;
      
      readToken (TOKEN_TYPE_COMMA);
    }

    // store parameters in symbol table (reverse (right-to-left) order)
    for (int i = param_list.size () - 1; i > 0; i--)
      symbol_table.addSymbol (param_list[i], 1, SYMBOL_TYPE_PARAM);
    // record parameter count
    func_table.setFunc (curr_scope, param_list.size ());
  }

  readToken (TOKEN_TYPE_CLOSE_PAREN);

  // parse the function body
  readToken (TOKEN_TYPE_OPEN_BRACE);
  parseBlock ();
  
  // return to global scope
  curr_scope = GLOBAL_SCOPE;
}
