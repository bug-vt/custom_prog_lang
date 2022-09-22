#include <fstream>
#include <iostream>
#include <sstream>
#include "instruction.hpp"

using std::string;
using std::cout;
using std::endl;
using std::string;
using std::ofstream;
using std::ifstream;
using std::stringstream;


string readHeader (ifstream &binary);
string readInstrStream (ifstream &binary);
string readStringTable (ifstream &binary);
string readFuncTable (ifstream &binary);


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


int main (int argc, char **argv)
{
  if (argc < 2)
  {
    cout << "usage: decoder executable_file <--short>" << endl;
    exit (1);
  }

  bool show_less = false;
  if (argc > 2)
  {
    if (string (argv[2]) == "--short")
      show_less = true;
  }

  string out = "";
  ifstream binary (argv[1]);

  out = readHeader (binary);
  if (!show_less)
    cout << out << endl;

  cout << readInstrStream (binary) << endl;

  out = readStringTable (binary);
  if (!show_less)
    cout << out << endl;

  out = readFuncTable (binary);
  if (!show_less)
    cout << out << endl;

  binary.close ();

  return 0;
}


string readHeader (ifstream &binary)
{
  // read header
  TestHeader header;
  stringstream out;

  char id[4];
  id[3] = '\0';
  binary.read ((char *) &id, 3);
  out << "Header:" << endl
      << string (id) << endl;


  binary.read (&header.ver_major, sizeof (char));
  binary.read (&header.ver_minor, sizeof (char));
  binary.read ((char *) &header.stack_size, sizeof (int));
  binary.read ((char *) &header.global_data_size, sizeof (int));
  binary.read (&header.is_main_func_present, sizeof (char));
  binary.read ((char *) &header.main_func_index, sizeof (int));

  out << (int) header.ver_major << " "
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
        << (int) op_count;
    if (op_count > 0)
      out << " ";
    else
      out << endl;

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
