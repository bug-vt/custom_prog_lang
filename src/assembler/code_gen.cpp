#include "code_gen.hpp"

using std::ostream;
using std::endl;

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

void CodeGen::writeSymbolTable ()
{

}

void CodeGen::writeLabelTable ()
{
}

void CodeGen::writeFuncTable ()
{
}

void CodeGen::writeStringTable ()
{
}

void CodeGen::writeInstrStream ()
{
  int size = instr_stream.size ();
  output.write ((char *) &size, sizeof (size));

  for (int i = 0; i < size; i++)
  {
    // write opcode
    char opcode = instr_stream[i].opcode;
    output.write (&opcode, sizeof (opcode));

    // write operand count 
    char op_count = instr_stream[i].op_list.size ();
    output.write (&op_count, sizeof (op_count));
  }
}
