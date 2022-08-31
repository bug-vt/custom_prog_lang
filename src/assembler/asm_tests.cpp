#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_COLOUR_NONE
#include "catch.hpp"
#include "asm_lexer.hpp"

std::unordered_map<std::string, InstrLookup> InstrLookupTable::instr_lookup;


TEST_CASE ("Basic instruction lookup", "[instr_lookup]")
{
  InstrLookupTable::init ();
  REQUIRE (InstrLookupTable::isInstr ("sub"));
  REQUIRE (InstrLookupTable::isInstr ("mov"));
  REQUIRE (InstrLookupTable::isInstr ("call"));
  REQUIRE (InstrLookupTable::isInstr ("push"));
  REQUIRE_FALSE (InstrLookupTable::isInstr ("ad"));
  REQUIRE_FALSE (InstrLookupTable::isInstr ("addi"));
}

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

TEST_CASE ("Basic identifier lexing", "[lexer]")
{
  std::string input = " hello    x _y    n123  ";
  
  AsmLexer lexer (input);
  REQUIRE (lexer.getNextToken () == TOKEN_TYPE_IDENT);
  REQUIRE (lexer.getCurrLexeme () == "hello");
  REQUIRE (lexer.getNextToken () == TOKEN_TYPE_IDENT);
  REQUIRE (lexer.getCurrLexeme () == "x");
  REQUIRE (lexer.getNextToken () == TOKEN_TYPE_IDENT);
  REQUIRE (lexer.getCurrLexeme () == "_y");
  REQUIRE (lexer.getNextToken () == TOKEN_TYPE_IDENT);
  REQUIRE (lexer.getCurrLexeme () == "n123");
}

TEST_CASE ("Lexing reserved word and instruction mnemonic", "[lexer]")
{
  InstrLookupTable::init ();
  std::string input = " mov    _x12 var   func   ";
  
  AsmLexer lexer (input);
  REQUIRE (lexer.getNextToken () == TOKEN_TYPE_INSTR);
  REQUIRE (lexer.getCurrLexeme () == "mov");
  REQUIRE (lexer.getNextToken () == TOKEN_TYPE_IDENT);
  REQUIRE (lexer.getCurrLexeme () == "_x12");
  REQUIRE (lexer.getNextToken () == TOKEN_TYPE_VAR);
  REQUIRE (lexer.getCurrLexeme () == "var");
  REQUIRE (lexer.getNextToken () == TOKEN_TYPE_FUNC);
  REQUIRE (lexer.getCurrLexeme () == "func");
}

TEST_CASE ("Basic delim lexing", "[lexer]")
{
  std::string input = " :    , [   ]   ";
  
  AsmLexer lexer (input);
  REQUIRE (lexer.getNextToken () == TOKEN_TYPE_COLON);
  REQUIRE (lexer.getCurrLexeme () == ":");
  REQUIRE (lexer.getNextToken () == TOKEN_TYPE_COMMA);
  REQUIRE (lexer.getCurrLexeme () == ",");
  REQUIRE (lexer.getNextToken () == TOKEN_TYPE_OPEN_BRACKET);
  REQUIRE (lexer.getCurrLexeme () == "[");
  REQUIRE (lexer.getNextToken () == TOKEN_TYPE_CLOSE_BRACKET);
  REQUIRE (lexer.getCurrLexeme () == "]");
}
