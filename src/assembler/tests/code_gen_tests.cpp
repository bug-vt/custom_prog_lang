#include "catch.hpp"
#include "../asm_parser.hpp"
#include <sys/wait.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <sstream>

using std::string;
using std::cout;
using std::endl;
using std::string;
using std::ofstream;
using std::ifstream;
using std::stringstream;

#define TEST_OUT_FILE "test_output.executable"

struct TestHeader
{
  char ver_major;
  char ver_minor;
  int stack_size;
  int global_data_size;
  char is_main_func_present;
  int main_func_index;
};

struct TestOp
{
    OpType type;
    union
    {
        int int_literal;
        float float_literal;
        int str_table_index;    // string table index
        int instr_index;        // instruction stream index
        int stack_index;        // stack index
        int func_index;         // function table index
        int reg;                // register code
    };
    int offset_index;           // stack index of the offset variable
};

int testCodeGen (string input)
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
    code_gen.writeHeader ();
    code_gen.writeInstrStream ();
    out_file.close ();

    exit (EXIT_SUCCESS);
  }
  // parent
  int status;
  wait (&status);
  int ret_val = WEXITSTATUS (status);

  return ret_val;
}

string readHeader (ifstream &binary)
{
  // read header
  TestHeader header;
  binary.read ((char *) &header, sizeof (TestHeader));

  stringstream out;
  out << "Header:" << endl
      << (int) header.ver_major << " "
      << (int) header.ver_minor << endl
      << header.stack_size << " "
      << header.global_data_size << " "
      << (bool) header.is_main_func_present << " "
      << header.main_func_index << endl;

  return out.str ();
}

string readInstrStream (ifstream &binary)
{
  stringstream out;
  // read size of instruction stream
  int size;
  binary.read ((char *) &size, sizeof (int));

  out << "Instruction stream:" << endl
      << size << endl;

  // read each instruction
  for (int i = 0; i < size; i++)
  {
    // read opcode and operand count
    char opcode, op_count;
    binary.read (&opcode, sizeof (char));
    binary.read (&op_count, sizeof (char));
   
    out << (int) opcode << " "
        << (int) op_count << " ";

    // read list of operands that are used in this instruction
    for (int op_index = 0; op_index < op_count; op_index++)
    {
      TestOp op;
      binary.read ((char *) &op, sizeof (TestOp));

      out << op.type << " "
          << op.int_literal << " "
          << op.offset_index;
      
      if (op_index < op_count - 1)
        out << " ";
      else
        out << endl;
    }
  }

  return out.str ();
}

bool testInstrStream (string expected)
{
  ifstream binary (TEST_OUT_FILE);

  readHeader (binary);
  string instr_stream = readInstrStream (binary);

  binary.close ();
  return instr_stream == expected;
}

TEST_CASE ("Generating header", "[code_gen]")
{
  string input = "\n\nfunc myFunc\n"
                 "{ \n"
                 "var xyz\n"
                 "mov xyz, 42 \n"
                 "}";
  string expected = "Header:\n"
                    "0 1\n"
                    "1024 0 0 -1\n";

  REQUIRE (testCodeGen (input) == EXIT_SUCCESS);

  ifstream binary (TEST_OUT_FILE);

  string header = readHeader (binary);
  REQUIRE (header == expected);

  binary.close ();
}


TEST_CASE ("Generating instruction", "[code_gen]")
{
  string input = "\n\nfunc myFunc\n"
                 "{ \n"
                 "var t\n"
                 "add t, 42 \n"
                 "}";
  string expected = "Instruction stream:\n"
                    "1\n"
                    "1 2 3 -2 0 0 42 0\n";

  REQUIRE (testCodeGen (input) == EXIT_SUCCESS);
  REQUIRE (testInstrStream (expected));
}

TEST_CASE ("Generating multiple instructions", "[code_gen]")
{
  string input = "\n\nfunc myFunc\n"
                 "{ \n"
                 "here:\n"
                 "var x\n"
                 "mov x, 9 \n"
                 "inc x \n"
                 "je x, 5, here \n"
                 "}";
  string expected = "Instruction stream:\n"
                    "3\n"
                    "0 2 3 -2 0 0 9 0\n"
                    "8 1 3 -2 0\n"
                    "20 3 3 -2 0 0 5 0 5 0 0\n";

  REQUIRE (testCodeGen (input) == EXIT_SUCCESS);
  REQUIRE (testInstrStream (expected));
}
