#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_COLOUR_NONE
#include "catch.hpp"
#include "asm_lexer.hpp"

TEST_CASE ("Basic integer lexing", "[lexer]")
{
  std::string input = "         1    333 5  ";
  
  AsmLexer lexer (input);
  REQUIRE (lexer.getNextToken () == TOKEN_TYPE_INT);
  REQUIRE (lexer.getCurrLexeme () == "1");
  REQUIRE (lexer.getNextToken () == TOKEN_TYPE_INT);
  REQUIRE (lexer.getCurrLexeme () == "333");
  REQUIRE (lexer.getNextToken () == TOKEN_TYPE_INT);
  REQUIRE (lexer.getCurrLexeme () == "5");
}

TEST_CASE ("Basic float lexing", "[lexer]")
{
  std::string input = "2.996    .3 1.8450  ";
  
  AsmLexer lexer (input);
  REQUIRE (lexer.getNextToken () == TOKEN_TYPE_FLOAT);
  REQUIRE (lexer.getCurrLexeme () == "2.996");
  REQUIRE (lexer.getNextToken () == TOKEN_TYPE_FLOAT);
  REQUIRE (lexer.getCurrLexeme () == ".3");
  REQUIRE (lexer.getNextToken () == TOKEN_TYPE_FLOAT);
  REQUIRE (lexer.getCurrLexeme () == "1.8450");
}

