#include "catch.hpp"
#include "../parser.hpp"
#include <sys/wait.h>
#include <unistd.h>
#include <fstream>

using std::ifstream;
using std::string;
using std::stringstream;

int testParse (string input)
{
  pid_t pid = fork ();
  // child
  if (pid == 0)
  {
    Parser parser (input);
    parser.parse ();
    exit (EXIT_SUCCESS);
  }
  // parent
  int status;
  wait (&status);
  int ret_val = WEXITSTATUS (status);

  return ret_val;
}

TEST_CASE ("Empty statements/blocks parsing", "[parser]")
{
  SECTION ("Empty statement")
  {
    string input = "\n\n\n;";

    REQUIRE (testParse (input) == EXIT_SUCCESS);
  }

  SECTION ("Empty block")
  {
    string input = "\n\n\n { \n }";

    REQUIRE (testParse (input) == EXIT_SUCCESS);
  }

  SECTION ("Nested empty statements/blocks")
  {
    string input = "{;} ;;; {}{}{} ; {;;}";

    REQUIRE (testParse (input) == EXIT_SUCCESS);
  }
}

TEST_CASE ("Basic function directive parsing", "[parser]")
{
  SECTION ("No parameter")
  {
    string input = "\n\nfunc myFunc ()\n { \n }";

    REQUIRE (testParse (input) == EXIT_SUCCESS);
  }

  SECTION ("One parameter")
  {
    string input = "func myFunc (param0)\n {  }";

    REQUIRE (testParse (input) == EXIT_SUCCESS);
  }

  SECTION ("Multiple parameters")
  {
    string input = "func myFunc (param0, param1, param2)\n {  }";

    REQUIRE (testParse (input) == EXIT_SUCCESS);
  }
}

TEST_CASE ("Function parsing error", "[parser]")
{
  SECTION ("No function name")
  {
    string input = "\n\nfunc 123 ()\n { \n }";
    REQUIRE (testParse (input) == EXIT_FAILURE);
  }

  SECTION ("No open brace")
  {
    string input = "\n\nfunc myFunc ()\n \n }";
    REQUIRE (testParse (input) == EXIT_FAILURE);
  }

  SECTION ("No close brace")
  {
    string input = "\n\nfunc myFunc ()\n \n {\n ";
    REQUIRE (testParse (input) == EXIT_FAILURE);
  }

  SECTION ("Two open brace")
  {
    string input = "\n\nfunc myFunc ()\n \n {\n { \n }";
    REQUIRE (testParse (input) == EXIT_FAILURE);
  }

  SECTION ("No parenthesis")
  {
    string input = "\n\nfunc myFunc \n \n {\n} ";
    REQUIRE (testParse (input) == EXIT_FAILURE);
  }

  SECTION ("No comma")
  {
    string input = "func myFunc (param0 param1) \n {\n} ";
    REQUIRE (testParse (input) == EXIT_FAILURE);
  }
}


TEST_CASE ("Basic var/var[] parsing", "[parser]")
{
  string input = "var xyz;   \n"
                 "    var array[40] ;\n"
                 "  var arr [ 30 ];";
  REQUIRE (testParse (input) == EXIT_SUCCESS);
}

TEST_CASE ("var/var[] parsing error", "[parser]")
{
  SECTION ("No identifier")
  {
    string input = "var 123;   \n";
    REQUIRE (testParse (input) == EXIT_FAILURE);
  }

  SECTION ("Two identifier")
  {
    string input = "  var x y;   \n";
    REQUIRE (testParse (input) == EXIT_FAILURE);
  }

  SECTION ("No open bracket")
  {
    string input = "  var z 33];   \n";
    REQUIRE (testParse (input) == EXIT_FAILURE);
  }

  SECTION ("No close bracket")
  {
    string input = "  var z [33;   \n";
    REQUIRE (testParse (input) == EXIT_FAILURE);
  }

  SECTION ("No index between brackets")
  {
    string input = "  var z [] ;  \n";
    REQUIRE (testParse (input) == EXIT_FAILURE);
  }

  SECTION ("No semi-colon")
  {
    string input = "  var xyz   \n";
    REQUIRE (testParse (input) == EXIT_FAILURE);
  }
}

/*
TEST_CASE ("parsing global variable as operand", "[parser]")
{
  string input = "var GLOBAL_VAR\n" 
                 "func someFunc\n"
                 "{ \n"
                 "add GLOBAL_VAR, 2\n"
                 "}";

  REQUIRE (testParse (input) == EXIT_SUCCESS);
}

TEST_CASE ("Test forward referencing", "[parser]")
{
  SECTION ("forward referencing label")
  {
    string input = "func someFunc\n"
                   "{ \n"
                   "jmp hello\n"
                   "push 2\n"
                   "hello:\n"
                   "}";

    REQUIRE (testParse (input) == EXIT_SUCCESS);
  }

  SECTION ("forward referencing function")
  {
    string input = "func someFunc\n"
                   "{ \n"
                   "call otherFunc\n"
                   "}\n"
                   "func otherFunc\n"
                   "{ \n"
                   "var tmp\n"
                   "mov tmp, 88\n"
                   "}\n";

    REQUIRE (testParse (input) == EXIT_SUCCESS);
  }
}

TEST_CASE ("Instruction parsing error", "[parser]")
{
  SECTION ("Incorrect add instruction")
  {
    string input = "\n\nfunc myFunc\n"
                   "{ \n"
                   "add 2, 2 \n"
                   "}";

    REQUIRE (testParse (input) == EXIT_FAILURE);
  }

  SECTION ("Incorrect sub instruction")
  {
    string input = "\n\nfunc myFunc\n"
                   "{ \n"
                   "var y\n"
                   "sub y, :\n"
                   "}";

    REQUIRE (testParse (input) == EXIT_FAILURE);
  }

  SECTION ("Calling undefined variable")
  {
    string input = "\n\nfunc myFunc\n"
                   "{ \n"
                   "mul xzy, 10\n"
                   "}";

    REQUIRE (testParse (input) == EXIT_FAILURE);
  }

  SECTION ("Calling undefined function")
  {
    string input = "\n\nfunc myFunc\n"
                   "{ \n"
                   "call noFunc\n"
                   "}";

    REQUIRE (testParse (input) == EXIT_FAILURE);
  }

  SECTION ("Incorrect use of ref instruction")
  {
    string input = "\n\nfunc myFunc\n"
                   "{ \n"
                   "var x\n"
                   "ref x, 2\n"
                   "}";

    REQUIRE (testParse (input) == EXIT_FAILURE);
  }
}

TEST_CASE ("Parsing file", "[parser]")
{
  ifstream input ("../example/example.casm");
  REQUIRE (input.good ());

  stringstream buffer;
  buffer << input.rdbuf ();

  REQUIRE (testParse (buffer.str ()) == EXIT_SUCCESS);
}
*/
// ----------------------------------------------------------------
// Tests for tables

TEST_CASE ("Test function table", "[func_table]")
{
  FuncTable func_table;
  REQUIRE (func_table.addFunc ("myFunc1") == 1);

  SECTION ("Trying to add function with same name")
  {
    REQUIRE (func_table.addFunc ("myFunc1") == -1);
  }

  SECTION ("Get function that exists inside table")
  {
    FuncInfo func_info;
    REQUIRE_NOTHROW (func_info = func_table.getFunc ("myFunc1"));
    REQUIRE (func_table.getFuncIndex ("myFunc1") == 1);
    REQUIRE (func_info.param_count == 0);
  }

  SECTION ("Get function that does NOT exists inside table")
  {
    REQUIRE (func_table.getFuncIndex ("noFunc") == -1);
  }

  SECTION ("Set parameters and local data size")
  {
    func_table.setFunc (1, 2);
    FuncInfo func_info = func_table.getFunc ("myFunc1");
    REQUIRE (func_info.param_count == 2);
  }

  SECTION ("Test situation when caller try to modify content inside table")
  {
    FuncInfo target_info;
    REQUIRE_NOTHROW (target_info = func_table.getFunc ("myFunc1"));
    REQUIRE (target_info.param_count == 0);
    target_info.param_count = 77;

    FuncInfo same_info;
    REQUIRE_NOTHROW (same_info = func_table.getFunc ("myFunc1"));
    REQUIRE (same_info.param_count == 0);
  }
}
