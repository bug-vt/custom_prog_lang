#include "catch.hpp"
#include "../parser.hpp"
#include <sys/wait.h>
#include <unistd.h>
#include <fstream>
#include "../ast_printer.hpp"
#include "../emitter.hpp"

using std::ifstream;
using std::string;
using std::stringstream;
using std::vector;

int testParse (string input, string expected="")
{
  pid_t pid = fork ();
  // child
  if (pid == 0)
  {
    Parser parser (input);
    vector<Stmt*> statements = parser.parse ();

    AstPrinter printer;
    string parse_tree = printer.print (statements);
    if (expected != "")
      REQUIRE (expected == parse_tree); 

    Emitter emitter;
    emitter.walkAst (statements);

    exit (EXIT_SUCCESS);
  }
  // parent
  int status;
  wait (&status);
  int ret_val = WEXITSTATUS (status);

  return ret_val;
}

TEST_CASE ("Empty blocks parsing", "[parser]")
{
  SECTION ("Empty block")
  {
    string input = "\n\n\n { \n }";

    REQUIRE (testParse (input) == EXIT_SUCCESS);
  }

  SECTION ("Nested empty blocks")
  {
    string input = "{  {{}}{}  {}}";

    REQUIRE (testParse (input) == EXIT_SUCCESS);
  }
}


TEST_CASE ("Basic function declration parsing", "[parser]")
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

  SECTION ("Using parameter inside body")
  {
    string input = "func myFunc (param0)"
                   "{"  
                   "  var local = param0;"
                   "}";

    REQUIRE (testParse (input) == EXIT_SUCCESS);
  }
}

TEST_CASE ("Function declration parsing error", "[parser]")
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

  SECTION ("Declaring outside global")
  {
    string input = "{ func myFunc (param0) \n {\n} }";
    REQUIRE (testParse (input) == EXIT_FAILURE);
  }
}

TEST_CASE ("Function call parsing", "[parser]")
{
  SECTION ("Calling function")
  {
    string input = "func myFunc ()"
                   "{"  
                   "  print 123;"
                   "}"
                   "func main ()"
                   "{"
                   "  myFunc ();"
                   "}";
    REQUIRE (testParse (input) == EXIT_SUCCESS);
  }
}

TEST_CASE ("Function call parsing error", "[parser]")
{
  SECTION ("Too few arguments")
  {
    string input = "func myFunc (a)"
                   "{"  
                   "  print a;"
                   "}"
                   "func main ()"
                   "{"
                   "  myFunc ();"
                   "}";
    REQUIRE (testParse (input) == EXIT_FAILURE);
  }

  SECTION ("Too many arguments")
  {
    string input = "func myFunc (b)"
                   "{"  
                   "  print b;"
                   "}"
                   "func main ()"
                   "{"
                   "  myFunc (1, 2, 3);"
                   "}";
    REQUIRE (testParse (input) == EXIT_FAILURE);
  }

  SECTION ("Undefined function")
  {
    string input = "func myFunc (b)"
                   "{"  
                   "  print b;"
                   "}"
                   "func main ()"
                   "{"
                   "  yourFunc (1);"
                   "}";
    REQUIRE (testParse (input) == EXIT_FAILURE);
  }

  SECTION ("Invalid use of function")
  {
    string input = "func myFunc (b)"
                   "{"  
                   "  print b;"
                   "}"
                   "func main ()"
                   "{"
                   "  print myFunc;"
                   "}";
    REQUIRE (testParse (input) == EXIT_FAILURE);
  }

  SECTION ("Name conflict with function")
  {
    string input = "func myFunc (b)"
                   "{"  
                   "  print b;"
                   "}"
                   "func main ()"
                   "{"
                   "  var myFunc = 123;"
                   "}";
    REQUIRE (testParse (input) == EXIT_FAILURE);
  }

  SECTION ("Name conflict with variable")
  {
    string input = "var myFunc = 1;" 
                   "func myFunc (b)"
                   "{"  
                   "  print b;"
                   "}"
                   "func main ()"
                   "{"
                   "  print 0;"
                   "}";
    REQUIRE (testParse (input) == EXIT_FAILURE);
  }
}


TEST_CASE ("Basic var/var[] parsing", "[parser]")
{
  string input = "var xyz;   \n"
                 "    var array[40] ;\n"
                 "  var arr [ 30 ];";
  string expected = "(Var xyz)\n"
                    "(Var array)\n"
                    "(Var arr)\n";

  REQUIRE (testParse (input, expected) == EXIT_SUCCESS);
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

  SECTION ("Undefined variable")
  {
    string input = "var x;"
                   "y = 12;";
    REQUIRE (testParse (input) == EXIT_FAILURE);
  }
}

TEST_CASE ("parsing global variable as operand", "[parser]")
{
  string input = "var GLOBAL_VAR = 42;\n" 
                 "func someFunc ()\n"
                 "{ \n"
                 "  var local = GLOBAL_VAR + 2;\n"
                 "}";
  string expected = "(Var GLOBAL_VAR 42)\n"
                    "(Func someFunc (Params)\n"
                    "(Var local (+ GLOBAL_VAR 2))\n"
                    ")\n";

  REQUIRE (testParse (input, expected) == EXIT_SUCCESS);
}

TEST_CASE ("Testing basic parsing", "[parser]")
{
  SECTION ("Assignments")
  {
    string input = "\n\nfunc myFunc ()\n"
                   "{ \n"
                   "  var y;\n"
                   "  var x = y = 123;\n"
                   "}";

    string expected = "(Func myFunc (Params)\n"
                      "(Var y)\n"
                      "(Var x (y= 123))\n"
                      ")\n";
    REQUIRE (testParse (input, expected) == EXIT_SUCCESS);
  }

  SECTION ("Left-associativity")
  {
    string input = "\n\nfunc myFunc ()\n"
                   "{ \n"
                   "  1 + 2 + 3 + 4 + 5;\n"
                   "}";

    string expected = "(Func myFunc (Params)\n"
                      "(Expr (+ (+ (+ (+ 1 2) 3) 4) 5))\n"
                      ")\n";
    REQUIRE (testParse (input, expected) == EXIT_SUCCESS);
  }
}

TEST_CASE ("Testing parsing error", "[parser]")
{
  SECTION ("No semi-colon")
  {
    string input = "\n\nfunc myFunc ()\n"
                   "{ \n"
                   "  var abc = 4\n"
                   "}";

    REQUIRE (testParse (input) == EXIT_FAILURE);
  }

  SECTION ("Equality instead of assignment")
  {
    string input = "\n\nfunc myFunc ()\n"
                   "{ \n"
                   "  var x == 33;\n"
                   "}";

    REQUIRE (testParse (input) == EXIT_FAILURE);
  }

  SECTION ("Incorrect l-value")
  {
    string input = "\n\nfunc myFunc ()\n"
                   "{ \n"
                   "  123 = 6;\n"
                   "}";

    REQUIRE (testParse (input) == EXIT_FAILURE);
  }

  SECTION ("Dangling block")
  {
    string input = "\n\nfunc myFunc ()\n"
                   "{ \n"
                   "  var hello = 0;\n";

    REQUIRE (testParse (input) == EXIT_FAILURE);
  }

  SECTION ("Expected expression for r-value")
  {
    string input = "\n\nfunc myFunc ()\n"
                   "{ \n"
                   "  var x = var y = 1;\n"
                   "}";

    REQUIRE (testParse (input) == EXIT_FAILURE);
  }
}

TEST_CASE ("Testing emitting error", "[parser]")
{
  SECTION ("break outside of loop")
  {
    string input = "\n\nfunc myFunc ()\n"
                   "{ \n"
                   "  break;\n"
                   "}";

    REQUIRE (testParse (input) == EXIT_FAILURE);
  }
}

TEST_CASE ("Parsing file", "[parser]")
{
  ifstream input ("../example/add.src");
  REQUIRE (input.good ());

  stringstream buffer;
  buffer << input.rdbuf ();

  REQUIRE (testParse (buffer.str ()) == EXIT_SUCCESS);
}

