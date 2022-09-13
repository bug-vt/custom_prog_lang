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
  // 1. version 
  // 2. stack size
  // 3. global data size
  // 4. flag for main function
  // 5. main function index
  output.write ((char *) &header, sizeof (Header)); 
}


void CodeGen::writeFuncTable ()
{
}

void CodeGen::writeStringTable ()
{
  // write total number of strings
  int size = str_table.size ();
  output.write ((char *) &size, sizeof (int)); 

  // write each string element form the string table
  for (int i = 0; i < size; i++)
  {
    string elem = str_table.at(i);
    int len = elem.length ();
    char str[len];
    strcpy (str, elem.c_str ());
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
