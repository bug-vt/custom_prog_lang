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

    Token readToken (TokenType req_token);
    // parsing methods
    Stmt* parseDeclaration ();
    Stmt* parseFunc ();
    Stmt* parseVar ();
    Stmt* parseStatement ();
    Stmt* parseReturnStatement ();
    Stmt* parseIfStatement ();
    Stmt* parseWhileStatement ();
    Stmt* parseForStatement ();
    Stmt* parseGotoStatement ();
    Stmt* parseExprStatement ();
    Stmt* parsePrintStatement ();
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
    Expr* parseCall ();
    Expr* parsePrimary ();
};

#endif
