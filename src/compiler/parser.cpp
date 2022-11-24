#include "parser.hpp"
#include <iostream>


using std::string;
using std::vector;

Parser::Parser (string raw_source)
{
  // initialize lexer
  lexer = Lexer (raw_source);
}

Token Parser::readToken (TokenType req_token)
{
  Token token = lexer.getNextToken ();
  if (token.type != req_token)
  {
    string msg = token2string (req_token) + " expected";
    throw std::runtime_error (msg); 
  }

  return token;
}

vector<Stmt*> Parser::parse ()
{
  lexer.reset ();

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
  try
  {
    if (lexer.getNextToken ().type == TOKEN_TYPE_VAR)
      return parseVar ();

    lexer.undoGetNextToken ();
    return parseStatement ();
  }
  catch (std::runtime_error &err)
  {
    lexer.error (err.what ());
  }

  throw std::runtime_error ("Should not reach here.");
}

Stmt* Parser::parseStatement ()
{
  Token first_token = lexer.getNextToken ();
  switch (first_token.type)
  {
    case TOKEN_TYPE_EOF:
      throw std::runtime_error ("Unexpected end of file");

    case TOKEN_TYPE_IF:
      return parseIfStatement ();

    case TOKEN_TYPE_PRINT:
      return parsePrintStatement ();

    case TOKEN_TYPE_OPEN_BRACE:
      return new Block (parseBlock ());

    default:
      break;
  }

  lexer.undoGetNextToken ();
  return parseExprStatement ();
}

Stmt* Parser::parseIfStatement ()
{
  readToken (TOKEN_TYPE_OPEN_PAREN);
  Expr* condition = parseExpr ();
  readToken (TOKEN_TYPE_CLOSE_PAREN);

  Stmt* thenBranch = parseStatement ();
  Stmt* elseBranch = nullptr;
  if (lexer.peekNextToken () == TOKEN_TYPE_ELSE)
  {
    readToken (TOKEN_TYPE_ELSE);
    elseBranch = parseStatement ();
  }

  return new If (condition, thenBranch, elseBranch);
}

Stmt* Parser::parseExprStatement ()
{
  Expr* expr = parseExpr ();
  readToken (TOKEN_TYPE_SEMICOLON);
  return new Expression (expr);
}

Stmt* Parser::parsePrintStatement ()
{
  Expr* expr = parseExpr ();
  readToken (TOKEN_TYPE_SEMICOLON);
  return new Print (expr);
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
 
  Expr* initializer = nullptr;
  // initialize when token '=' is present after declaration
  if (lexer.peekNextToken () == TOKEN_TYPE_ASSIGN)
  {
    readToken (TOKEN_TYPE_ASSIGN);
    initializer = parseExpr ();
  }

  readToken (TOKEN_TYPE_SEMICOLON);

  return new Var (ident, initializer, 0);
}

vector<Stmt*> Parser::parseBlock ()
{
  vector<Stmt*> statements;

  TokenType next_token = lexer.peekNextToken ();
  while (next_token != TOKEN_TYPE_CLOSE_BRACE && next_token != TOKEN_TYPE_EOF)
  {
    statements.push_back (parseDeclaration ());
    next_token = lexer.peekNextToken ();
  }

  readToken (TOKEN_TYPE_CLOSE_BRACE);
  return statements;
}


Expr* Parser::parseExpr ()
{
  return parseAssignment ();
}

Expr* Parser::parseAssignment ()
{
  Expr* expr = parseOr ();

  Token op = lexer.getNextToken ();
  if (op.type == TOKEN_TYPE_ASSIGN)
  {
    Expr* value = parseAssignment ();

    // l-value must be variable
    if (dynamic_cast<Variable*> (expr) != nullptr)
    {
      Token name = ((Variable*) expr)->name;
      return new Assign (name, value, 0);
    }
    throw std::runtime_error ("Invalid assignment target.");
  }

  lexer.undoGetNextToken ();
  return expr;
}

Expr* Parser::parseOr ()
{
  Expr* expr = parseAnd ();

  Token op = lexer.getNextToken ();
  while (op.type == TOKEN_TYPE_LOGICAL_OR)
  {
    Expr* right = parseAnd ();
    expr = new Logical (expr, op, right);
    op = lexer.getNextToken ();
  }

  lexer.undoGetNextToken ();
  return expr;
}

Expr* Parser::parseAnd ()
{
  Expr* expr = parseEquality ();

  Token op = lexer.getNextToken ();
  while (op.type == TOKEN_TYPE_LOGICAL_AND)
  {
    Expr* right = parseEquality ();
    expr = new Logical (expr, op, right);
    op = lexer.getNextToken ();
  }

  lexer.undoGetNextToken ();
  return expr;
}

Expr* Parser::parseEquality ()
{
  Expr* expr = parseComparison ();

  while (true)
  {
    Token op_token = lexer.getNextToken (); 
    if (op_token.type != TOKEN_TYPE_NOT_EQUAL 
        && op_token.type != TOKEN_TYPE_EQUAL)
    {
      lexer.undoGetNextToken ();
      break;
    }
    
    Expr *right = parseComparison ();
    expr = new Binary (expr, op_token, right);
  }

  return expr;
}

Expr* Parser::parseComparison ()
{
  Expr* expr = parseTerm ();

  while (true)
  {
    Token op_token = lexer.getNextToken (); 
    if (op_token.type != TOKEN_TYPE_GREATER 
        && op_token.type != TOKEN_TYPE_GREATER_EQUAL
        && op_token.type != TOKEN_TYPE_LESS 
        && op_token.type != TOKEN_TYPE_LESS_EQUAL)
    {
      lexer.undoGetNextToken ();
      break;
    }
    
    Expr *right = parseTerm ();
    expr = new Binary (expr, op_token, right);
  }

  return expr;
}

Expr* Parser::parseTerm ()
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

Expr* Parser::parseFactor ()
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

Expr* Parser::parseUnary ()
{
  Token op_token = lexer.getNextToken (); 
  if (op_token.type == TOKEN_TYPE_SUB 
      || op_token.type == TOKEN_TYPE_LOGICAL_NOT)
  {
    Expr *right = parseUnary ();
    return new Unary (op_token, right);
  }
  
  lexer.undoGetNextToken ();

  return parsePrimary ();
}

Expr* Parser::parsePrimary ()
{
  Token token = lexer.getNextToken (); 
  // Literal
  if (token.type == TOKEN_TYPE_FALSE 
      || token.type == TOKEN_TYPE_TRUE
      || token.type == TOKEN_TYPE_INT 
      || token.type == TOKEN_TYPE_FLOAT 
      || token.type == TOKEN_TYPE_STRING)
    return new Literal (token);

  // Variable
  if (token.type == TOKEN_TYPE_IDENT)
    return new Variable (token, 0);

  // Grouping
  if (token.type == TOKEN_TYPE_OPEN_PAREN)
  {
    Expr *expr = parseExpr ();
    readToken (TOKEN_TYPE_CLOSE_PAREN);
    return new Grouping (expr);
  }

  throw std::runtime_error ("Expected expression.");
}


