#include "catch.hpp"
#include "../asm_parser.hpp"
#include <sys/wait.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include "decoder.hpp"

using std::string;
using std::cout;
using std::endl;
using std::string;
using std::ofstream;
using std::ifstream;
using std::stringstream;

#define TEST_OUT_FILE "test_output.executable"

enum CodeGenFlag {GEN_HEADER = 1, 
                  GEN_INSTR = 2, 
                  GEN_STR_TABLE = 4, 
                  GEN_FUNC_TABLE = 8};


int testCodeGen (string input, int flags)
{
  pid_t pid = fork ();
  // child
  if (pid == 0)
  {
    AsmParser parser (input);
    parser.parse ();

    ofstream out_file (TEST_OUT_FILE);
    CodeGen code_gen = parser.createCodeGen (out_file);
    code_gen.setVersion (0, 1);
    if (flags & GEN_HEADER)
      code_gen.writeHeader ();
    if (flags & GEN_INSTR)
      code_gen.writeInstrStream ();
    if (flags & GEN_STR_TABLE)
      code_gen.writeStringTable ();
    if (flags & GEN_FUNC_TABLE)
      code_gen.writeFuncTable ();
    out_file.close ();

    exit (EXIT_SUCCESS);
  }
  // parent
  int status;
  wait (&status);
  int ret_val = WEXITSTATUS (status);

  return ret_val;
}



TEST_CASE ("Writing header", "[code_gen]")
{
  string input = "\n\nfunc myFunc\n"
                 "{ \n"
                 "var xyz\n"
                 "mov xyz, 42 \n"
                 "}";
  string expected = "Header:\n"
                    "BUG\n"
                    "0 1\n"
                    "1024 0 0 -1\n";

  REQUIRE (testCodeGen (input, GEN_HEADER) == EXIT_SUCCESS);

  ifstream binary (TEST_OUT_FILE);

  REQUIRE (Decoder::readHeader (binary) == expected);

  binary.close ();
}


TEST_CASE ("Writing a single instruction", "[code_gen]")
{
  string input = "\n\nfunc myFunc\n"
                 "{ \n"
                 "var t\n"
                 "add t, 42 \n"
                 "}";
  string expected = "Instruction stream:\n"
                    "2\n"
                    "4 2 3 -2 0 0 42 0\n"
                    "33 0\n";

  REQUIRE (testCodeGen (input, GEN_INSTR) == EXIT_SUCCESS);
  ifstream binary (TEST_OUT_FILE);

  REQUIRE (Decoder::readInstrStream (binary) == expected);

  binary.close ();
}

TEST_CASE ("Writing a instruction with float operand", "[code_gen]")
{
  string input = "\n\nfunc myFunc\n"
                 "{ \n"
                 "var k\n"
                 "mov k, 0.123 \n"
                 "}";
  string expected = "Instruction stream:\n"
                    "2\n"
                    "0 2 3 -2 0 1 0.123 0\n"
                    "33 0\n";

  REQUIRE (testCodeGen (input, GEN_INSTR) == EXIT_SUCCESS);
  ifstream binary (TEST_OUT_FILE);

  REQUIRE (Decoder::readInstrStream (binary) == expected);

  binary.close ();
}

TEST_CASE ("Writing a print instruction", "[code_gen]")
{
  string input = "\n\nfunc myFunc\n"
                 "{ \n"
                 "var str\n"
                 "mov str, \"hello world!\"\n"
                 "print str\n"
                 "}";
  string expected = "Instruction stream:\n"
                    "3\n"
                    "0 2 3 -2 0 2 0 0\n"
                    "36 1 3 -2 0\n"
                    "33 0\n";

  REQUIRE (testCodeGen (input, GEN_INSTR) == EXIT_SUCCESS);
  ifstream binary (TEST_OUT_FILE);

  REQUIRE (Decoder::readInstrStream (binary) == expected);

  binary.close ();
}

TEST_CASE ("Writing a lw instruction", "[code_gen]")
{
  string input = "\n\nfunc myFunc\n"
                 "{ \n"
                 "var x\n"
                 "var y\n"
                 "lw x, 12, 0\n"
                 "lw y, x, 2\n"
                 "}";
  string expected = "Instruction stream:\n"
                    "3\n"
                    "2 3 3 -2 0 0 12 0 0 0 0\n"
                    "2 3 3 -3 0 3 -2 0 0 2 0\n"
                    "33 0\n";

  REQUIRE (testCodeGen (input, GEN_INSTR) == EXIT_SUCCESS);
  ifstream binary (TEST_OUT_FILE);

  REQUIRE (Decoder::readInstrStream (binary) == expected);

  binary.close ();
}

TEST_CASE ("Writing a ref instruction", "[code_gen]")
{
  string input = "\n\nfunc myFunc\n"
                 "{ \n"
                 "var x\n"
                 "var y\n"
                 "ref y, x\n"
                 "}";
  string expected = "Instruction stream:\n"
                    "2\n"
                    "1 2 3 -3 0 3 -2 0\n"
                    "33 0\n";

  REQUIRE (testCodeGen (input, GEN_INSTR) == EXIT_SUCCESS);
  ifstream binary (TEST_OUT_FILE);

  REQUIRE (Decoder::readInstrStream (binary) == expected);

  binary.close ();
}

TEST_CASE ("Using return value register", "[code_gen]")
{
  string input = "\n\nfunc myFunc\n"
                 "{ \n"
                 "mov _retVal, 33\n"
                 "}";
  string expected = "Instruction stream:\n"
                    "2\n"
                    "0 2 7 0 0 0 33 0\n"
                    "33 0\n";

  REQUIRE (testCodeGen (input, GEN_INSTR) == EXIT_SUCCESS);
  ifstream binary (TEST_OUT_FILE);

  REQUIRE (Decoder::readInstrStream (binary) == expected);

  binary.close ();
}

TEST_CASE ("Writing branch instruction", "[code_gen]")
{
  string input = "\n\nfunc myFunc\n"
                 "{ \n"
                 "var x\n"
                 "mov x, 9 \n"
                 "mov x, 9 \n"
                 "here:\n"
                 "inc x \n"
                 "je x, 5, here \n"
                 "}";
  string expected = "Instruction stream:\n"
                    "5\n"
                    "0 2 3 -2 0 0 9 0\n"
                    "0 2 3 -2 0 0 9 0\n"
                    "11 1 3 -2 0\n"
                    "29 3 3 -2 0 0 5 0 5 2 0\n"
                    "33 0\n";

  REQUIRE (testCodeGen (input, GEN_INSTR) == EXIT_SUCCESS);
  ifstream binary (TEST_OUT_FILE);

  REQUIRE (Decoder::readInstrStream (binary) == expected);

  binary.close ();
}

TEST_CASE ("Writing branch instruction with comparison instruction", "[code_gen]")
{
  string input = "\n\nfunc myFunc\n"
                 "{ \n"
                 "var x\n"
                 "var y\n"
                 "mov x, 9 \n"
                 "here:\n"
                 "sne y, 5, x\n"
                 "je y, 1, here \n"
                 "}";
  string expected = "Instruction stream:\n"
                    "4\n"
                    "0 2 3 -2 0 0 9 0\n"
                    "23 3 3 -3 0 0 5 0 3 -2 0\n"
                    "29 3 3 -3 0 0 1 0 5 1 0\n"
                    "33 0\n";

  REQUIRE (testCodeGen (input, GEN_INSTR) == EXIT_SUCCESS);
  ifstream binary (TEST_OUT_FILE);

  REQUIRE (Decoder::readInstrStream (binary) == expected);

  binary.close ();
}


TEST_CASE ("Writing string table", "[code_gen]")
{
  string input = "\n\nfunc myFunc\n"
                 "{ \n"
                 "var str\n"
                 "mov str, \"Hello World!\\n\"\n"
                 "}";
  string expected = "String table:\n"
                    "1\n"
                    "13 Hello World!\n\n";

  REQUIRE (testCodeGen (input, GEN_STR_TABLE) == EXIT_SUCCESS);

  ifstream binary (TEST_OUT_FILE);

  REQUIRE (Decoder::readStringTable (binary) == expected);

  binary.close ();
}

TEST_CASE ("Writing function table", "[code_gen]")
{
  string input = "\n\nfunc myFunc\n"
                 "{ \n"
                 "param x\n"
                 "var str\n"
                 "}";
  string expected = "Function table:\n"
                    "2\n"
                    "-1 -1 -1\n"
                    "0 1 1\n";

  REQUIRE (testCodeGen (input, GEN_FUNC_TABLE) == EXIT_SUCCESS);

  ifstream binary (TEST_OUT_FILE);

  REQUIRE (Decoder::readFuncTable (binary) == expected);

  binary.close ();
}

TEST_CASE ("Writing multiple functions", "[code_gen]")
{
  string input = "func myFunc\n"
                 "{ \n"
                 "param x\n"
                 "var hello\n"
                 "mov hello, 1\n"
                 "}\n"
                 "func main\n"
                 "{ \n"
                 "var hello\n"
                 "}";
  string expected = "Function table:\n"
                    "3\n"
                    "-1 -1 -1\n"
                    "0 1 1\n"
                    "2 0 1\n";

  REQUIRE (testCodeGen (input, GEN_FUNC_TABLE) == EXIT_SUCCESS);

  ifstream binary (TEST_OUT_FILE);

  REQUIRE (Decoder::readFuncTable (binary) == expected);

  binary.close ();
}
