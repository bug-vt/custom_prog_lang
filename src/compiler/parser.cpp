#include "parser.hpp"
#include <vector>
#include <iostream>

#define GLOBAL_SCOPE 0

using std::string;
using std::vector;

Parser::Parser (string raw_source)
{
  // initialize lexer
  lexer = Lexer (raw_source);
  // Add two temporary variables to simulate general-purpose registers 
  Symbol tmp_var0 ("_T0", GLOBAL_SCOPE);
  Symbol tmp_var1 ("_T1", GLOBAL_SCOPE);
  tmp0_sym_index = symbol_table.addSymbol (tmp_var0, 1, SYMBOL_TYPE_VAR);
  tmp1_sym_index = symbol_table.addSymbol (tmp_var1, 1, SYMBOL_TYPE_VAR);
}

CodeGen Parser::createCodeGen ()
{
  CodeGen code_gen;
  // put tables 
  code_gen.sym_table = symbol_table;
  code_gen.func_table = func_table;

  return code_gen;
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

    case TOKEN_TYPE_VAR:
      parseVar ();
      break;

    case TOKEN_TYPE_INT:
      parseExpr ();
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
    for (int i = param_list.size () - 1; i >= 0; i--)
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

void Parser::parseVar ()
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

  // add symbol to the symbol table and check for redefinition
  Symbol symbol (ident, curr_scope); 
  if (symbol_table.addSymbol (symbol, size, SYMBOL_TYPE_VAR) == -1)
    lexer.error ("Identifier with same name already exists inside the same scope");

  readToken (TOKEN_TYPE_SEMICOLON);
}

void Parser::parseExpr ()
{
  lexer.undoGetNextToken ();
  parseSubExpr ();
}

void Parser::parseSubExpr ()
{
  int instr_index;
  Token op_token;
  
  FuncInfo& curr_func = func_table.at (curr_scope);
  // first operand
  lexer.getNextToken (); 
  instr_index = curr_func.icode.addInstr (INSTR_PUSH);
  curr_func.icode.addIntOp (instr_index, stoi (lexer.getCurrLexeme ()));
  while (true)
  {
    Token token = lexer.getNextToken (); 
    if (token != TOKEN_TYPE_ADD && token != TOKEN_TYPE_SUB)
    {
      lexer.undoGetNextToken ();
      break;
    }
    
    op_token = token;

    // second operand
    lexer.getNextToken (); 
    instr_index = curr_func.icode.addInstr (INSTR_PUSH);
    curr_func.icode.addIntOp (instr_index, stoi (lexer.getCurrLexeme ()));

    // pop the first operand to _t1
    instr_index = curr_func.icode.addInstr (INSTR_POP);
    curr_func.icode.addVarOp (instr_index, tmp1_sym_index);

    // pop the second operand to _t2
    instr_index = curr_func.icode.addInstr (INSTR_POP);
    curr_func.icode.addVarOp (instr_index, tmp0_sym_index);
    
    int op_instr;
    switch (op_token)
    {
      case TOKEN_TYPE_ADD:
        op_instr = INSTR_ADD;
        break;
      case TOKEN_TYPE_SUB:
        op_instr = INSTR_SUB;
        break;
    }

    instr_index = curr_func.icode.addInstr (op_instr);
    curr_func.icode.addVarOp (instr_index, tmp0_sym_index);
    curr_func.icode.addVarOp (instr_index, tmp1_sym_index);

    // push the result
    instr_index = curr_func.icode.addInstr (INSTR_PUSH);
    curr_func.icode.addVarOp (instr_index, tmp0_sym_index);
  }
}

