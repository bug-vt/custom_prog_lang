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
    // lexer
    Lexer lexer;

    // methods
    Token readToken (TokenType req_token);
    Stmt* parseStatement ();
    Stmt* parseIfStatement ();
    Stmt* parseExprStatement ();
    Stmt* parsePrintStatement ();
    Stmt* parseDeclaration ();
    Stmt* parseVar ();
    std::vector<Stmt*> parseBlock ();
    Expr* parseExpr ();
    Expr* parseAssignment ();
    Expr* parseOr ();
    Expr* parseAnd ();
    Expr* parseEquality ();
    Expr* parseComparison ();
    Expr* parseTerm ();
    Expr* parseFactor ();
    Expr* parseUnary ();
    Expr* parsePrimary ();
};

#endif
