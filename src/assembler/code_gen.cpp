#include "code_gen.hpp"

using std::ostream;
using std::endl;

CodeGen::CodeGen (ostream &out_file) : output (out_file)
{
}

void CodeGen::setVersion (char ver_major, char ver_minor)
{
  this->ver_major = ver_major;
  this->ver_minor = ver_minor;
}

void CodeGen::writeHeader ()
{
  // write assembler version (2 bytes)
  output.write (&ver_major, sizeof (ver_major)); 
  output.write (&ver_minor, sizeof (ver_minor)); 

  // write header:
  // 1. stack size (4 bytes)
  // 2. global data size (4 bytes)
  // 3. flag for main function (1 bytes)
  // 4. main function index (4 bytes)
  output.write ((char *) &header, sizeof (header)); 
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
}
