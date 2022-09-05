#include "catch.hpp"
#include "../asm_parser.hpp"
#include <sys/wait.h>
#include <unistd.h>

using std::string;

int testParse (string input)
{
  pid_t pid = fork ();
  // child
  if (pid == 0)
  {
    AsmParser parser (input);
    parser.parse ();
    exit (EXIT_SUCCESS);
  }
  // parent
  int status;
  wait (&status);
  int ret_val = WEXITSTATUS (status);

  return ret_val;
}

TEST_CASE ("Basic function directive parsing", "[parser]")
{
  string input = "\n\nfunc myFunc\n { \n }";

  REQUIRE (testParse (input) == EXIT_SUCCESS);
}

TEST_CASE ("Function parsing error", "[parser]")
{
  SECTION ("No function name")
  {
    string input = "\n\nfunc 123\n { \n }";
    REQUIRE (testParse (input) == EXIT_FAILURE);
  }

  SECTION ("No open brace")
  {
    string input = "\n\nfunc myFunc\n \n }";
    REQUIRE (testParse (input) == EXIT_FAILURE);
  }

  SECTION ("No close brace")
  {
    string input = "\n\nfunc myFunc\n \n {\n ";
    REQUIRE (testParse (input) == EXIT_FAILURE);
  }

  SECTION ("Two open brace")
  {
    string input = "\n\nfunc myFunc\n \n {\n { \n }";
    REQUIRE (testParse (input) == EXIT_FAILURE);
  }
}

TEST_CASE ("Basic var/var[] parsing", "[parser]")
{
  string input = "var xyz   \n"
                 "    var array[40] \n"
                 "  var arr [ 30 ]";
  REQUIRE (testParse (input) == EXIT_SUCCESS);
}

TEST_CASE ("var/var[] parsing error", "[parser]")
{
  SECTION ("No identifier")
  {
    string input = "var 123   \n";
    REQUIRE (testParse (input) == EXIT_FAILURE);
  }

  SECTION ("Two identifier")
  {
    string input = "  var x y   \n";
    REQUIRE (testParse (input) == EXIT_FAILURE);
  }

  SECTION ("No open bracket")
  {
    string input = "  var z 33]   \n";
    REQUIRE (testParse (input) == EXIT_FAILURE);
  }

  SECTION ("No close bracket")
  {
    string input = "  var z [33   \n";
    REQUIRE (testParse (input) == EXIT_FAILURE);
  }

  SECTION ("No index between brackets")
  {
    string input = "  var z []   \n";
    REQUIRE (testParse (input) == EXIT_FAILURE);
  }
}

TEST_CASE ("Basic param parsing", "[parser]")
{
  string input = "\n\nfunc myFunc\n"
                 "{ \n"
                 "param x \n"
                 "param y \n"
                 "}";

  REQUIRE (testParse (input) == EXIT_SUCCESS);
}

TEST_CASE ("param parsing error", "[parser]")
{
  SECTION ("Defined in global scope")
  {
    string input = "param x \n"
                   "func myFunc\n"
                   "{ \n"
                   "}";
    REQUIRE (testParse (input) == EXIT_FAILURE);
  }

  SECTION ("No identifier")
  {
    string input = "\n\nfunc myFunc\n"
                   "{ \n"
                   "param 123 \n"
                   "}";
    REQUIRE (testParse (input) == EXIT_FAILURE);
  }
}

TEST_CASE ("Basic line label parsing", "[parser]")
{
  SECTION ("Placing new line after label")
  {
    string input = "func myFunc\n"
                   "{ \n"
                   "start:\n"
                   "        var hello \n"
                   "}";
    REQUIRE (testParse (input) == EXIT_SUCCESS);
  }

  SECTION ("Not placing new line after label")
  {
    string input = "func myFunc\n"
                   "{ \n"
                   "start:  var hello \n"
                   "}";
    REQUIRE (testParse (input) == EXIT_SUCCESS);
  }
}


TEST_CASE ("Basic input parsing", "[parser]")
{
  string input = "\n\nfunc myFunc\n"
                 "{ \n"
                 "start:\n"
                 "        var hello \n"
                 "        var world[ 42] \n"
                 "        param x \n"
                 "finish: param y \n"
                 "}";

  REQUIRE (testParse (input) == EXIT_SUCCESS);
}

// ----------------------------------------------------------------
// Tests for tables

TEST_CASE ("Test function table", "[func_table]")
{
  FuncTable func_table;
  REQUIRE (func_table.addFunc ("myFunc1", 0) == 1);

  SECTION ("Trying to add function with same name")
  {
    REQUIRE (func_table.addFunc ("myFunc1", 0) == -1);
  }

  SECTION ("Get function that exists inside table")
  {
    FuncInfo func_info;
    REQUIRE_NOTHROW (func_info = func_table.getFunc ("myFunc1"));
    REQUIRE (func_info.func_index == 1);
    REQUIRE (func_info.entry_point == 0);
  }

  SECTION ("Get function that does NOT exists inside table")
  {
    REQUIRE_THROWS (func_table.getFunc ("noFunc"));
  }

  SECTION ("Set parameters and local data size")
  {
    func_table.setFunc (1, 2, 3);
    FuncInfo func_info = func_table.getFunc ("myFunc1");
    REQUIRE (func_info.param_count == 2);
    REQUIRE (func_info.local_data_size == 3);
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

