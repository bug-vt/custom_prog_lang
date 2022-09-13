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

struct HeaderTest
{
  char ver_major;
  char ver_minor;
  int stack_size;
  int global_data_size;
  char is_main_func_present;
  int main_func_index;
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
    out_file.close ();

    exit (EXIT_SUCCESS);
  }
  // parent
  int status;
  wait (&status);
  int ret_val = WEXITSTATUS (status);

  return ret_val;
}

string readBinary ()
{
  ifstream binary (TEST_OUT_FILE);

  // read header
  HeaderTest header;
  binary.read ((char *) &header, sizeof (HeaderTest));

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

  string output = readBinary ();
  REQUIRE (output == expected);
}
