/*
 * This implement assembler, which generate byte code for custom VM.
 * It takes assembly code that was adapted from the Alex Varanese's
 * XtremeScript assembly.
 * The output executable is formatted as follow:
 * 1. Header
 * 2. Instruction stream
 * 3. String table
 * 4. Function table
 */

#include <fstream>
#include <sstream>
#include "asm_parser.hpp"
#include "error.hpp"

using std::ifstream;
using std::ofstream;
using std::string;
using std::stringstream;

#define VERSION_MAJOR 1
#define VERSION_MINOR 0

string loadAssemblyFile (char *file_name);


int main (int argc, char **argv)
{
  if (argc < 2)
    exitOnError ("usage: assembler assembly_file <output_file_name>");
 
  string out_file_name = "out.exec";
  if (argc > 2)
    out_file_name = string (argv[2]) + ".exec";

  string raw_source = loadAssemblyFile (argv[1]);
  AsmParser parser (raw_source);
  parser.parse ();

  ofstream output_file (out_file_name);
  CodeGen code_gen = parser.createCodeGen (output_file);
  code_gen.setVersion (VERSION_MAJOR, VERSION_MINOR);   
  code_gen.writeHeader ();
  code_gen.writeInstrStream ();
  code_gen.writeStringTable ();
  code_gen.writeFuncTable ();

  output_file.close ();

  return 0;
}

string loadAssemblyFile (char *file_name)
{
  // open the input file
  ifstream input_file (file_name);
  
  if (input_file.fail ())
    exitOnError ("fail to open given input file: " + string (file_name));

  // load assembly code into memory
  stringstream buffer;
  buffer << input_file.rdbuf ();

  // close the file
  input_file.close ();

  return buffer.str ();
}

