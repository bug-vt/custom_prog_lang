#include "catch.hpp"
#include "../asm_parser.hpp"
#include <sys/wait.h>
#include <unistd.h>
#include <fstream>
#include <iostream>

using std::cout;
using std::endl;
using std::string;

int testCodeGen (string input)
{
  pid_t pid = fork ();
  // child
  if (pid == 0)
  {
    AsmParser parser (input);
    parser.parse ();
    CodeGen code_gen = parser.createCodeGen (cout);
    code_gen.setVersion (0, 1);
    code_gen.writeHeader ();
    exit (EXIT_SUCCESS);
  }
  // parent
  int status;
  wait (&status);
  int ret_val = WEXITSTATUS (status);

  return ret_val;
}

TEST_CASE ("Generating header", "[code_gen]")
{
  string input = "\n\nfunc myFunc\n"
                 "{ \n"
                 "var xyz\n"
                 "mov xyz, 42 \n"
                 "}";

  REQUIRE (testCodeGen (input) == EXIT_SUCCESS);
}
