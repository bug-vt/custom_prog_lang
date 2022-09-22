#include "script.hpp"
#include <fstream>

using std::string;
using std::ifstream;

void Script::loadScript (string file_name)
{
  ifstream binary (file_name);
  // load header
  
  // load instruction stream
  
  // load string table

  // load function table
 
  binary.close ();
}

