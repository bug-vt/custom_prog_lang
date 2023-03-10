#include "catch.hpp"
#include "../asm_lexer.hpp"
#include <fstream>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>

using std::ifstream;
using std::stringstream;
using std::cout;
using std::endl;
using std::string;



TEST_CASE ("Basic instruction lookup", "[instr_lookup]")
{
  InstrLookupTable instr_table = InstrLookupTable::instance ();
  REQUIRE (instr_table.isInstr ("sub"));
  REQUIRE (instr_table.isInstr ("mov"));
  REQUIRE (instr_table.isInstr ("call"));
  REQUIRE (instr_table.isInstr ("push"));
  REQUIRE_FALSE (instr_table.isInstr ("ad"));
  REQUIRE_FALSE (instr_table.isInstr ("addi"));
}

TEST_CASE ("Basic integer lexing", "[lexer]")
{
  string input = "         1    333 5  ";
  
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
  string input = "2.996    .3 1.8450  ";
  
  AsmLexer lexer (input);
  REQUIRE (lexer.getNextToken () == TOKEN_TYPE_FLOAT);
  REQUIRE (lexer.getCurrLexeme () == "2.996");
  REQUIRE (lexer.getNextToken () == TOKEN_TYPE_FLOAT);
  REQUIRE (lexer.getCurrLexeme () == ".3");
  REQUIRE (lexer.getNextToken () == TOKEN_TYPE_FLOAT);
  REQUIRE (lexer.getCurrLexeme () == "1.8450");
}

TEST_CASE ("Negative intger/float lexing", "[lexer]")
{
  string input = "-1    -1.0 -4.2   -99 -.123  ";
  
  AsmLexer lexer (input);
  REQUIRE (lexer.getNextToken () == TOKEN_TYPE_INT);
  REQUIRE (lexer.getCurrLexeme () == "-1");
  REQUIRE (lexer.getNextToken () == TOKEN_TYPE_FLOAT);
  REQUIRE (lexer.getCurrLexeme () == "-1.0");
  REQUIRE (lexer.getNextToken () == TOKEN_TYPE_FLOAT);
  REQUIRE (lexer.getCurrLexeme () == "-4.2");
  REQUIRE (lexer.getNextToken () == TOKEN_TYPE_INT);
  REQUIRE (lexer.getCurrLexeme () == "-99");
  REQUIRE (lexer.getNextToken () == TOKEN_TYPE_FLOAT);
  REQUIRE (lexer.getCurrLexeme () == "-.123");
}

TEST_CASE ("Basic identifier lexing", "[lexer]")
{
  string input = " hello    x _y    n123  ";
  
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
  string input = " xy[123]    qq [ 2 ]   hi[ 42] ";
  
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
  string input = " mov    _x12 var   func   ";
  
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
  string input = " :    , [   ]   ";
  
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
  string input = " \"Hello\"  \"to \\\"World\\\"\" \"fall $ number  @sun\"";
  
  AsmLexer lexer (input);
  REQUIRE (lexer.getNextToken () == TOKEN_TYPE_STRING);
  REQUIRE (lexer.getCurrLexeme () == "Hello");
  REQUIRE (lexer.getNextToken () == TOKEN_TYPE_STRING);
  REQUIRE (lexer.getCurrLexeme () == "to \"World\"");
  REQUIRE (lexer.getNextToken () == TOKEN_TYPE_STRING);
  REQUIRE (lexer.getCurrLexeme () == "fall $ number  @sun");
}

TEST_CASE ("String lexing with escape characters", "[lexer]")
{
  string input = " \"newline\\n tab\\t quote\\\" \"";
  
  AsmLexer lexer (input);
  REQUIRE (lexer.getNextToken () == TOKEN_TYPE_STRING);
  REQUIRE (lexer.getCurrLexeme () == "newline\n tab\t quote\" ");
}

TEST_CASE ("Lexing comment", "[lexer]")
{
  string input = "; this is comment \n \
                  xyz ; another comment\n \
                  123";
  
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
  string input = "; comment first line\n \
                  var xyz\n \
                  param qqq \n \
                  je 4.8, 7.3";
  
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
  CHECK (lexer.getCurrLexeme () == "je");
  CHECK (lexer.getNextToken () == TOKEN_TYPE_FLOAT);
  CHECK (lexer.getCurrLexeme () == "4.8");
  CHECK (lexer.getNextToken () == TOKEN_TYPE_COMMA);
  CHECK (lexer.getCurrLexeme () == ",");
  CHECK (lexer.getNextToken () == TOKEN_TYPE_FLOAT);
  CHECK (lexer.getCurrLexeme () == "7.3");
}

TEST_CASE ("Peek next token", "[lexer]")
{
  string input = "  ahead 123 rewind ";
  
  AsmLexer lexer (input);
  CHECK (lexer.getNextToken () == TOKEN_TYPE_IDENT);
  CHECK (lexer.getCurrLexeme () == "ahead");
  CHECK (lexer.peekNextToken () == TOKEN_TYPE_INT);
  CHECK (lexer.getCurrLexeme () == "ahead");
  CHECK (lexer.getNextToken () == TOKEN_TYPE_INT);
  CHECK (lexer.getCurrLexeme () == "123");
}

TEST_CASE ("Lexing Invalid token", "[lexer]")
{
  string input = "  7ident  1.2. ident! @  -  -12-\n";
  
  AsmLexer lexer (input);
  CHECK (lexer.getNextToken () == TOKEN_TYPE_INVALID);
  CHECK (lexer.getCurrLexeme () == "7ident");
  CHECK (lexer.getNextToken () == TOKEN_TYPE_INVALID);
  CHECK (lexer.getCurrLexeme () == "1.2.");
  CHECK (lexer.getNextToken () == TOKEN_TYPE_INVALID);
  CHECK (lexer.getCurrLexeme () == "ident!");
  CHECK (lexer.getNextToken () == TOKEN_TYPE_INVALID);
  CHECK (lexer.getCurrLexeme () == "@");
  CHECK (lexer.getNextToken () == TOKEN_TYPE_INVALID);
  CHECK (lexer.getCurrLexeme () == "-");
  CHECK (lexer.getNextToken () == TOKEN_TYPE_INVALID);
  CHECK (lexer.getCurrLexeme () == "-12-");
}

TEST_CASE ("Lexing until EOF", "[lexer]")
{
  string input = "  123  hello world \n";

  AsmLexer lexer (input);
  lexer.getNextToken ();
  lexer.getNextToken ();
  lexer.getNextToken ();
  lexer.getNextToken ();
  CHECK (lexer.getNextToken () == TOKEN_TYPE_EOF);
  CHECK (lexer.getCurrLexeme () == "");
}

TEST_CASE ("Test displaying error", "[lexer]")
{
  string input = "var xyz\n \
                  \t\t\t\tparam 0qqq \n \
                  jg 4.8, 7.3";

  pid_t pid = fork ();
  // child
  if (pid == 0)
  {
    cout << "Testing invalid token. Should display error message:" << endl;
    AsmLexer lexer (input);
    Token curr_token = lexer.getNextToken ();
    while (curr_token != TOKEN_TYPE_EOF)
    {
      if (curr_token == TOKEN_TYPE_INVALID)
        lexer.error ("code error");

      curr_token = lexer.getNextToken ();
    }
    exit (EXIT_SUCCESS);
  }
  // parent
  int status;
  wait (&status);
  int ret_val = WEXITSTATUS (status);
  REQUIRE (ret_val == EXIT_FAILURE);
}

TEST_CASE ("Lexing file", "[lexer]")
{
  ifstream input ("../example/example.casm");
  REQUIRE (input.good ());
  stringstream buffer;
  buffer << input.rdbuf ();

  AsmLexer lexer (buffer.str ());
  // loop until all tokens are consumed.
  Token curr_token = lexer.getNextToken ();
  while (curr_token != TOKEN_TYPE_EOF)
  {
    /*
    string lexeme = lexer.getCurrLexeme ();
    if (lexeme == "\n")
      lexeme = "\\n";

    // display lexmeme and token type for each token
    cout << lexeme << "\t\t"
         << token2string (curr_token) << endl;
    */
    CHECK (curr_token != TOKEN_TYPE_INVALID);

    // get next token
    curr_token = lexer.getNextToken ();
  }
}
