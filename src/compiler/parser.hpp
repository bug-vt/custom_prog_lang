#ifndef PARSER_HPP
#define PARSER_HPP

#include "lexer.hpp"
#include "token.hpp"
#include "expr.hpp"
#include "stmt.hpp"

class Parser
{
  public:
    Parser () { }
    Parser (std::string raw_source);
    std::vector<Stmt*> parse ();
    
  private:
    // variables for tracking current function
    int curr_scope;

    // lexer
    Lexer lexer;

    // methods
    Token readToken (TokenType req_token);
    Stmt* parseStatement ();
    Stmt* parseExprStatement ();
    Stmt* parsePrintStatement ();
    Stmt* parseDeclaration ();
    Stmt* parseVar ();
    std::vector<Stmt*> parseBlock ();
    Expr* parseAssignment ();
    Expr* parseExpr ();
    Expr* parseTerm ();
    Expr* parseFactor ();
    Expr* parseUnary ();
    Expr* parsePrimary ();
};

#endif
