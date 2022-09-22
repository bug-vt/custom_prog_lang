#include "code_gen.hpp"
#include <cstring>

using std::ostream;
using std::endl;
using std::string;

CodeGen::CodeGen (ostream &out_file) : output (out_file)
{
}

void CodeGen::setVersion (char ver_major, char ver_minor)
{
  this->header.ver_major = ver_major;
  this->header.ver_minor = ver_minor;
}

void CodeGen::writeHeader ()
{
  // write header:
  // 1. executable id (3 bytes)
  char id[] = "BUG";
  output.write (id, strlen (id));
  // 2. version (2 bytes) 
  output.write (&header.ver_major, sizeof (char));
  output.write (&header.ver_minor, sizeof (char));
  // 3. stack size  (4 bytes)
  output.write ((char *) &header.stack_size, sizeof (int));
  // 4. global data size (4 bytes)
  output.write ((char *) &header.global_data_size, sizeof (int));
  // 5. flag for main function (1 byte)
  output.write (&header.is_main_func_present, sizeof (char));
  // 6. main function index (4 bytes)
  output.write ((char *) &header.main_func_index, sizeof (int));
}


void CodeGen::writeFuncTable ()
{
  // write total number of function 
  int size = func_table.size ();
  output.write ((char *) &size, sizeof (int)); 

  // write each function form the function table
  for (int i = 0; i < size; i++)
  {
    FuncInfo func = func_table.at (i);
    int entry_point = func.entry_point;
    int param_count = func.param_count;
    int local_data_size = func.local_data_size;
    output.write ((char *) &entry_point, sizeof (int));
    output.write ((char *) &param_count, sizeof (int));
    output.write ((char *) &local_data_size, sizeof (int));
  }
}

void CodeGen::writeStringTable ()
{
  // write total number of strings
  int size = str_table.size ();
  output.write ((char *) &size, sizeof (int)); 

  // write each string element form the string table
  for (int i = 0; i < size; i++)
  {
    string elem = str_table.at (i);
    int len = elem.length ();
    char str[len];
    strncpy (str, elem.c_str (), len);
    output.write ((char *) &len, sizeof (int));
    output.write (str, len);
  }
}

void CodeGen::writeInstrStream ()
{
  // write total number of instructions
  int size = instr_stream.size ();
  output.write ((char *) &size, sizeof (int));

  // write each instruction from the instruction stream 
  for (int i = 0; i < size; i++)
  {
    // write opcode
    char opcode = instr_stream[i].opcode;
    output.write (&opcode, sizeof (char));

    // write operand count 
    char op_count = instr_stream[i].op_list.size ();
    output.write (&op_count, sizeof (char));

    // write each operand from the operand list
    for (int op_index = 0; op_index < op_count; op_index++)
    {
      Op operand = instr_stream[i].op_list[op_index];
      output.write ((char *) &operand, sizeof (Op)); 
    }
  }
}
