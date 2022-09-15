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

enum CodeGenFlag {GEN_HEADER = 1, 
                  GEN_INSTR = 2, 
                  GEN_STR_TABLE = 4, 
                  GEN_FUNC_TABLE = 8};

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

string readStringTable (ifstream &binary)
{
  stringstream out;
  // read size of string table 
  int size;
  binary.read ((char *) &size, sizeof (int));

  out << "String table:" << endl
      << size << endl;

  // read each string
  for (int i = 0; i < size; i++)
  {
    // read string length 
    int len;
    binary.read ((char *) &len, sizeof (int));
   
    char str[len + 1]; 
    str[len] = '\0';
    binary.read (str, len);

    out << len << " "
        << string (str) << endl;
  }

  return out.str ();
}

string readFuncTable (ifstream &binary)
{
  stringstream out;
  // read size of function table 
  int size;
  binary.read ((char *) &size, sizeof (int));

  out << "Function table:" << endl
      << size << endl;

  // read each function 
  for (int i = 0; i < size; i++)
  {
    int entry_point;
    int param_count;
    int local_data_size;
    binary.read ((char *) &entry_point, sizeof (int));
    binary.read ((char *) &param_count, sizeof (int));
    binary.read ((char *) &local_data_size, sizeof (int));

    out << entry_point << " "
        << param_count << " "
        << local_data_size << endl;
  }

  return out.str ();
}


TEST_CASE ("Writing header", "[code_gen]")
{
  string input = "\n\nfunc myFunc\n"
                 "{ \n"
                 "var xyz\n"
                 "mov xyz, 42 \n"
                 "}";
  string expected = "Header:\n"
                    "0 1\n"
                    "1024 0 0 -1\n";

  REQUIRE (testCodeGen (input, GEN_HEADER) == EXIT_SUCCESS);

  ifstream binary (TEST_OUT_FILE);

  REQUIRE (readHeader (binary) == expected);

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
                    "1\n"
                    "1 2 3 -2 0 0 42 0\n";

  REQUIRE (testCodeGen (input, GEN_INSTR) == EXIT_SUCCESS);
  ifstream binary (TEST_OUT_FILE);

  REQUIRE (readInstrStream (binary) == expected);

  binary.close ();
}

TEST_CASE ("Writing multiple instructions", "[code_gen]")
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

  REQUIRE (testCodeGen (input, GEN_INSTR) == EXIT_SUCCESS);
  ifstream binary (TEST_OUT_FILE);

  REQUIRE (readInstrStream (binary) == expected);

  binary.close ();
}


TEST_CASE ("Writing string table", "[code_gen]")
{
  string input = "\n\nfunc myFunc\n"
                 "{ \n"
                 "var str\n"
                 "mov str, \"Hello World!\"\n"
                 "}";
  string expected = "String table:\n"
                    "1\n"
                    "12 Hello World!\n";

  REQUIRE (testCodeGen (input, GEN_STR_TABLE) == EXIT_SUCCESS);

  ifstream binary (TEST_OUT_FILE);

  REQUIRE (readStringTable (binary) == expected);

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

  REQUIRE (readFuncTable (binary) == expected);

  binary.close ();
}
