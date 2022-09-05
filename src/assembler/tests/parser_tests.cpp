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

TEST_CASE ("Directive parsing error", "[parser]")
{
  string input = "\n\nfunc 123\n { \n }";

  REQUIRE (testParse (input) == EXIT_FAILURE);
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

