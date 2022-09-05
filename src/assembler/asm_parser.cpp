#include "asm_parser.hpp"
#include "error.hpp"

#define GLOBAL_SCOPE 0

using std::string;

AsmParser::AsmParser (string raw_source)
{
  lexer = AsmLexer (raw_source);

  global_data_size = 0;
  instr_stream_size = 0;
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

    case TOKEN_TYPE_VAR:
      parseVar ();
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

  // instruction immediately after the current one is the function's entry point
  // which is equal to the current instruction stream size
  int entry_point = instr_stream_size;
  // add function to the function table and check for redefinition
  int func_index = func_table.addFunc (lexer.getCurrLexeme (), entry_point); 
  if (func_index == -1)
    exitOnCodeError ("Function was already defined somewhere else", lexer);

  // change scope to the function
  // and reinitialize variables that used for tracking function
  curr_scope = func_index;
  curr_func_param_size = 0;
  curr_func_local_data_size = 0;

  while (lexer.peekNextToken () == TOKEN_TYPE_NEWLINE)
    readToken (TOKEN_TYPE_NEWLINE);

  // start the code block with opening curly brace.
  readToken (TOKEN_TYPE_OPEN_BRACE);

  // parse function body
  parseBlock ();

  // make room for extra instruction 
  // since all function must be append with ret instruction. 
  instr_stream_size++;
  // finish setting up function
  func_table.setFunc (curr_scope, curr_func_param_size, curr_func_local_data_size);

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

void AsmParser::parseVar ()
{
  readToken (TOKEN_TYPE_IDENT);

  string ident = lexer.getCurrLexeme ();
  // for now, consider as a variable (array have size >= 1)
  int size = 1;

  // if next token is open bracket,
  // verify if the identifier is an array and change to identified size
  if (lexer.peekNextToken () == TOKEN_TYPE_OPEN_BRACKET)
  {
    readToken (TOKEN_TYPE_OPEN_BRACKET);

    readToken (TOKEN_TYPE_INT);
    size = stoi (lexer.getCurrLexeme ());
    
    readToken (TOKEN_TYPE_CLOSE_BRACKET);
  }

  // calculate variable's index inside stack
  // global variables reside on the bottom of the stack 
  // and indexed by positive stack index.
  int stack_index;
  if (curr_scope == GLOBAL_SCOPE)
    stack_index = global_data_size;
  // whereas local variables reside on the top of the stack
  // and indexed by negative stack index.
  // Offset 2 is needed since other function info need to be added 
  // on the top portion of the function stack frame.
  else
    stack_index = -(curr_func_local_data_size + 2);

  // add symbol to the symbol table and check for redefinition
  Symbol symbol (ident, curr_scope); 
  if (symbol_table.addSymbol (symbol, size, stack_index) == -1)
    exitOnCodeError ("Identifier with same name already exists inside the same scope", lexer);

  // grow the global or local data size
  if (curr_scope == GLOBAL_SCOPE)
    global_data_size += size;
  else
    curr_func_local_data_size += size;
  
  readToken (TOKEN_TYPE_NEWLINE);
}
