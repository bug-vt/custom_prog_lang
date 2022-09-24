#include "script.hpp"
#include "loader.hpp"
#include <fstream>

using std::vector;
using std::string;
using std::ifstream;


void Script::loadScript (string file_name)
{
  ifstream binary (file_name);

  Loader loader (binary);
  // load header
  loader.loadHeader (stack, global_data_size, is_main_func_present, main_func_index);
  
  // load instruction stream
  loader.loadInstrStream (instr_stream);

  // load string table
  loader.loadStringTable (str_table);

  // load function table
  loader.loadFuncTable (func_table);

 
  binary.close ();
}
