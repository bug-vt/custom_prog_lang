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

