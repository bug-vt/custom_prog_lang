#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_COLOUR_NONE
#include "catch.hpp"
#include "../asm_lexer.hpp"
#include <fstream>
#include <iostream>

using std::ifstream;
using std::stringstream;
using std::cout;
using std::endl;
using std::string;

std::unordered_map<std::string, InstrLookup> InstrLookupTable::instr_lookup;

string token2string (Token token)
{
  string out;
  switch (token)
  {
    case TOKEN_TYPE_EOF:
      out = "EOF";
      break;
    case TOKEN_TYPE_INT:
      out = "int";
      break;
    case TOKEN_TYPE_FLOAT:
      out = "float";
      break;
    case TOKEN_TYPE_STRING:
      out = "string";
      break;
    case TOKEN_TYPE_IDENT:
      out = "ident";
      break;
    case TOKEN_TYPE_COLON:
      out = "colon";
      break;
    case TOKEN_TYPE_OPEN_BRACKET:
      out = "open bracket";
      break;
    case TOKEN_TYPE_CLOSE_BRACKET:
      out = "close bracket";
      break;
    case TOKEN_TYPE_COMMA:
      out = "comma";
      break;
    case TOKEN_TYPE_OPEN_BRACE:
      out = "open brace";
      break;
    case TOKEN_TYPE_CLOSE_BRACE:
      out = "close brace";
      break;
    case TOKEN_TYPE_NEWLINE:
      out = "new line";
      break;
    case TOKEN_TYPE_INSTR:
      out = "instruction";
      break;
    case TOKEN_TYPE_SETSTACKSIZE:
      out = "setStackSize";
      break;
    case TOKEN_TYPE_VAR:
      out = "var";
      break;
    case TOKEN_TYPE_FUNC:
      out = "func";
      break;
    case TOKEN_TYPE_PARAM:
      out = "param";
      break;
    case TOKEN_TYPE_REG_RETVAL:
      out = "_retVal";
      break;
    default:
      out = "invalid";
  }
  return out;
}


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

TEST_CASE ("Baisc array lexing", "[lexer]")
{
  std::string input = " xy[123]    qq [ 2 ]   hi[ 42] ";
  
  AsmLexer lexer (input);
  REQUIRE (lexer.getNextToken () == TOKEN_TYPE_IDENT);
  REQUIRE (lexer.getCurrLexeme () == "xy");
  REQUIRE (lexer.getNextToken () == TOKEN_TYPE_OPEN_BRACKET);
  REQUIRE (lexer.getCurrLexeme () == "[");
  REQUIRE (lexer.getNextToken () == TOKEN_TYPE_INT);
  REQUIRE (lexer.getCurrLexeme () == "123");
  REQUIRE (lexer.getNextToken () == TOKEN_TYPE_CLOSE_BRACKET);
  REQUIRE (lexer.getCurrLexeme () == "]");

  REQUIRE (lexer.getNextToken () == TOKEN_TYPE_IDENT);
  REQUIRE (lexer.getCurrLexeme () == "qq");
  REQUIRE (lexer.getNextToken () == TOKEN_TYPE_OPEN_BRACKET);
  REQUIRE (lexer.getCurrLexeme () == "[");
  REQUIRE (lexer.getNextToken () == TOKEN_TYPE_INT);
  REQUIRE (lexer.getCurrLexeme () == "2");
  REQUIRE (lexer.getNextToken () == TOKEN_TYPE_CLOSE_BRACKET);
  REQUIRE (lexer.getCurrLexeme () == "]");

  REQUIRE (lexer.getNextToken () == TOKEN_TYPE_IDENT);
  REQUIRE (lexer.getCurrLexeme () == "hi");
  REQUIRE (lexer.getNextToken () == TOKEN_TYPE_OPEN_BRACKET);
  REQUIRE (lexer.getCurrLexeme () == "[");
  REQUIRE (lexer.getNextToken () == TOKEN_TYPE_INT);
  REQUIRE (lexer.getCurrLexeme () == "42");
  REQUIRE (lexer.getNextToken () == TOKEN_TYPE_CLOSE_BRACKET);
  REQUIRE (lexer.getCurrLexeme () == "]");
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

TEST_CASE ("Basic string lexing", "[lexer]")
{
  std::string input = " \"Hello\"  \"to \\\"World\\\"\" \"fall $ number  @sun\"";
  
  AsmLexer lexer (input);
  REQUIRE (lexer.getNextToken () == TOKEN_TYPE_STRING);
  REQUIRE (lexer.getCurrLexeme () == "Hello");
  REQUIRE (lexer.getNextToken () == TOKEN_TYPE_STRING);
  REQUIRE (lexer.getCurrLexeme () == "to \"World\"");
  REQUIRE (lexer.getNextToken () == TOKEN_TYPE_STRING);
  REQUIRE (lexer.getCurrLexeme () == "fall $ number  @sun");
}

TEST_CASE ("Lexing comment", "[lexer]")
{
  std::string input = "; this is comment \n xyz ; another comment\n 123";
  
  AsmLexer lexer (input);
  REQUIRE (lexer.getNextToken () == TOKEN_TYPE_NEWLINE);
  REQUIRE (lexer.getCurrLexeme () == "\n");
  REQUIRE (lexer.getNextToken () == TOKEN_TYPE_IDENT);
  REQUIRE (lexer.getCurrLexeme () == "xyz");
  REQUIRE (lexer.getNextToken () == TOKEN_TYPE_NEWLINE);
  REQUIRE (lexer.getCurrLexeme () == "\n");
  REQUIRE (lexer.getNextToken () == TOKEN_TYPE_INT);
  REQUIRE (lexer.getCurrLexeme () == "123");
}

TEST_CASE ("Lexing multi-line input", "[lexer]")
{
  std::string input = "; comment first line\n \
                       var xyz\n \
                       param qqq \n \
                       jg 4.8, 7.3";
  
  AsmLexer lexer (input);
  CHECK (lexer.getNextToken () == TOKEN_TYPE_NEWLINE);
  CHECK (lexer.getCurrLexeme () == "\n");
  CHECK (lexer.getNextToken () == TOKEN_TYPE_VAR);
  CHECK (lexer.getCurrLexeme () == "var");
  CHECK (lexer.getNextToken () == TOKEN_TYPE_IDENT);
  CHECK (lexer.getCurrLexeme () == "xyz");
  CHECK (lexer.getNextToken () == TOKEN_TYPE_NEWLINE);
  CHECK (lexer.getCurrLexeme () == "\n");
  CHECK (lexer.getNextToken () == TOKEN_TYPE_PARAM);
  CHECK (lexer.getCurrLexeme () == "param");
  CHECK (lexer.getNextToken () == TOKEN_TYPE_IDENT);
  CHECK (lexer.getCurrLexeme () == "qqq");
  CHECK (lexer.getNextToken () == TOKEN_TYPE_NEWLINE);
  CHECK (lexer.getCurrLexeme () == "\n");
  CHECK (lexer.getNextToken () == TOKEN_TYPE_INSTR);
  CHECK (lexer.getCurrLexeme () == "jg");
  CHECK (lexer.getNextToken () == TOKEN_TYPE_FLOAT);
  CHECK (lexer.getCurrLexeme () == "4.8");
  CHECK (lexer.getNextToken () == TOKEN_TYPE_COMMA);
  CHECK (lexer.getCurrLexeme () == ",");
  CHECK (lexer.getNextToken () == TOKEN_TYPE_FLOAT);
  CHECK (lexer.getCurrLexeme () == "7.3");
}

TEST_CASE ("Look ahead and rewind", "[lexer]")
{
  std::string input = "  ahead 123 rewind ";
  
  AsmLexer lexer (input);
  CHECK (lexer.getNextToken () == TOKEN_TYPE_IDENT);
  CHECK (lexer.getCurrLexeme () == "ahead");
  CHECK (lexer.getNextToken () == TOKEN_TYPE_INT);
  CHECK (lexer.getCurrLexeme () == "123");
  lexer.rewindTokenStream ();
  CHECK (lexer.getCurrLexeme () == "ahead");
  CHECK (lexer.getNextToken () == TOKEN_TYPE_INT);
  CHECK (lexer.getCurrLexeme () == "123");
}

TEST_CASE ("Lexing Invalid token", "[lexer]")
{
  std::string input = "  7ident  1.2. ident! @ \n";
  
  AsmLexer lexer (input);
  CHECK (lexer.getNextToken () == TOKEN_TYPE_INVALID);
  CHECK (lexer.getCurrLexeme () == "7ident");
  CHECK (lexer.getNextToken () == TOKEN_TYPE_INVALID);
  CHECK (lexer.getCurrLexeme () == "1.2.");
  CHECK (lexer.getNextToken () == TOKEN_TYPE_INVALID);
  CHECK (lexer.getCurrLexeme () == "ident!");
  CHECK (lexer.getNextToken () == TOKEN_TYPE_INVALID);
  CHECK (lexer.getCurrLexeme () == "@");
}

TEST_CASE ("Lexing until EOF", "[lexer]")
{
  std::string input = "  123  hello world \n";

  AsmLexer lexer (input);
  lexer.getNextToken ();
  lexer.getNextToken ();
  lexer.getNextToken ();
  lexer.getNextToken ();
  CHECK (lexer.getNextToken () == TOKEN_TYPE_EOF);
  CHECK (lexer.getCurrLexeme () == "");
}

TEST_CASE ("Lexing file", "[lexer]")
{
  ifstream input ("example.assembly");
  stringstream buffer;
  buffer << input.rdbuf ();

  AsmLexer lexer (buffer.str ());
  // loop until all tokens are consumed.
  Token curr_token = lexer.getNextToken ();
  while (curr_token != TOKEN_TYPE_EOF)
  {
    string lexeme = lexer.getCurrLexeme ();
    if (lexeme == "\n")
      lexeme = "\\n";

    // display lexmeme and token type for each token
    cout << lexeme << "\t\t"
         << token2string (curr_token) << endl;

    // get next token
    curr_token = lexer.getNextToken ();
  }
}
