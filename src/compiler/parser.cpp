#include "parser.hpp"
#include <vector>
#include <iostream>
#include <cassert>

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

Token Parser::readToken (TokenType req_token)
{
  Token token = lexer.getNextToken ();
  if (token.type != req_token)
  {
    string msg = token2string (req_token) + " expected";
    lexer.error (msg); 
    // should not reach here
    assert (false);
  }

  return token;
}

vector<Stmt*> Parser::parse ()
{
  lexer.reset ();
  curr_scope = GLOBAL_SCOPE; 

  vector<Stmt*> statements;

  while (true)
  {
    // end of file is reached. End the parsing
    if (lexer.peekNextToken () == TOKEN_TYPE_EOF)
      break;
    // otherwise, go to next statement
    else
      statements.push_back (parseDeclaration ());
  }

  return statements;
}

Stmt* Parser::parseDeclaration ()
{
  if (lexer.getNextToken ().type == TOKEN_TYPE_VAR)
    return parseVar ();

  lexer.undoGetNextToken ();
  return parseStatement ();
}

Stmt* Parser::parseStatement ()
{
  if (lexer.getNextToken ().type == TOKEN_TYPE_EOF)
    lexer.error ("Unexpected end of file");

  lexer.undoGetNextToken ();
  return parseExprStatement ();
}

Stmt* Parser::parseExprStatement ()
{
  Expr* expr = parseExpr ();
  readToken (TOKEN_TYPE_SEMICOLON);
  return new Expression (expr);
}

Stmt* Parser::parseVar ()
{
  Token ident = readToken (TOKEN_TYPE_IDENT);

  // for now, consider as a variable (array have size >= 1)
  int size = 1;

  // if next token is open bracket,
  // verify if the identifier is an array and change to identified size
  if (lexer.peekNextToken () == TOKEN_TYPE_OPEN_BRACKET)
  {
    readToken (TOKEN_TYPE_OPEN_BRACKET);
    size = stoi (readToken (TOKEN_TYPE_INT).lexeme);
    readToken (TOKEN_TYPE_CLOSE_BRACKET);
  }

  // add symbol to the symbol table and check for redefinition
  Symbol symbol (ident.lexeme, curr_scope); 
  if (symbol_table.addSymbol (symbol, size, SYMBOL_TYPE_VAR) == -1)
    lexer.error ("Identifier with same name already exists inside the same scope");
 
  Expr* initializer = nullptr;
  // initialize when token '=' is present after declaration
  if (lexer.peekNextToken () == TOKEN_TYPE_ASSIGN)
  {
    readToken (TOKEN_TYPE_ASSIGN);
    initializer = parseExpr ();
  }

  readToken (TOKEN_TYPE_SEMICOLON);

  return new Var (ident, initializer);
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


Expr *Parser::parseExpr ()
{
  return parseTerm ();
}

Expr *Parser::parseTerm ()
{
  Expr *expr = parseFactor ();
  while (true)
  {
    Token op_token = lexer.getNextToken (); 
    if (op_token.type != TOKEN_TYPE_ADD && op_token.type != TOKEN_TYPE_SUB)
    {
      lexer.undoGetNextToken ();
      break;
    }
    
    Expr *right = parseFactor ();
    expr = new Binary (expr, op_token, right);
  }

  return expr;
}

Expr *Parser::parseFactor ()
{
  Expr *expr = parseUnary ();

  while (true)
  {
    Token op_token = lexer.getNextToken (); 
    if (op_token.type != TOKEN_TYPE_DIV && op_token.type != TOKEN_TYPE_MUL)
    {
      lexer.undoGetNextToken ();
      break;
    }
    
    Expr *right = parseUnary ();
    expr = new Binary (expr, op_token, right);
  }

  return expr;
}

Expr *Parser::parseUnary ()
{
  Token op_token = lexer.getNextToken (); 
  if (op_token.type == TOKEN_TYPE_SUB)
  {
    Expr *right = parseUnary ();
    return new Unary (op_token, right);
  }
  
  lexer.undoGetNextToken ();

  return parsePrimary ();
}

Expr *Parser::parsePrimary ()
{
  Token token = lexer.getNextToken (); 
  if (token.type == TOKEN_TYPE_INT)
    return new Literal (token);

  if (token.type == TOKEN_TYPE_IDENT)
    return new Variable (token);

  if (token.type == TOKEN_TYPE_OPEN_PAREN)
  {
    Expr *expr = parseExpr ();
    readToken (TOKEN_TYPE_CLOSE_PAREN);
    return new Grouping (expr);
  }

  lexer.error ("Expected expression.");
  assert (false);
  return nullptr;
}


