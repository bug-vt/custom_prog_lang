#include "code_gen.hpp"
#include <cstring>

#define GLOBAL_SCOPE 0

using std::ostream;
using std::endl;
using std::string;

void CodeGen::init (string input_file_name, string output_file_name)
{
  this->input_file_name = input_file_name;
  this->output_file_name = output_file_name;
}

void CodeGen::write ()
{
  output.open (output_file_name);
  if (output.fail ())
    throw std::runtime_error ("Fail to open output file");

  writeHeader ();
  output << "; ---------- Global variables -----------\n\n";
  writeScopeSymbols (GLOBAL_SCOPE, SYMBOL_TYPE_VAR);

  output << "; ------------- Functions ---------------\n\n";
  for (int i = 1; i < func_table.size (); i++)
  {
    writeFunc (func_table.at (i), i); 
    output << "\n\n";
  }

  output.close ();
}

void CodeGen::writeHeader ()
{
  // write header:
  // 1. output file name
  output << "; " << output_file_name << endl;
  // 2. source file name
  output << "; Source file: " << input_file_name << endl;
  // 3. version
  // 4. time stamp
  output << endl;
}

void CodeGen::writeScopeSymbols (int scope, int type)
{
  for (auto const& symbol : sym_table.symbol_table)
  {
    if (symbol.first.func_index == scope && symbol.second.type == type)
    {
      // insert tabbing for local variables
      if (scope != GLOBAL_SCOPE)
        output << "  ";
      if (type == SYMBOL_TYPE_PARAM)
        output << "param " << symbol.first.ident;
      else if (type == SYMBOL_TYPE_VAR)
      {
        output << "var " << symbol.first.ident;
        // array
        if (symbol.second.size > 1)
          output << "[" << symbol.second.size << "]";
      }

      output << endl;
    }
  }
}

void CodeGen::writeFunc (FuncInfo func, int func_index)
{
  output << "func " << func.func_name << endl;
  output << "{" << endl;

  writeScopeSymbols (func_index, SYMBOL_TYPE_PARAM);
  writeScopeSymbols (func_index, SYMBOL_TYPE_VAR);

  // write function body
  // Todo...
  
  output << "}" << endl;
}

