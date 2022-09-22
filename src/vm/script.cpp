#include "script.hpp"
#include <fstream>

using std::string;
using std::ifstream;

void Script::loadScript (string file_name)
{
  ifstream binary (file_name);
  // load header
  char id[4];
  id[3] = '\0';
  binary.read ((char *) &id, 3);
  if (string (id) != "BUG")
  {
    // load error
  }
  char ver_major, ver_minor;
  binary.read (&ver_major, sizeof (char));
  binary.read (&ver_minor, sizeof (char));
  if (ver_major != 1 || ver_minor != 0)
  {
    // load error
  }
  // read stack size
  binary.read ((char *) &stack.size, sizeof (int));
  // read global data size
  binary.read ((char *) &global_data_size, sizeof (int));
  // read main func info
  binary.read (&is_main_func_present, sizeof (char));
  binary.read ((char *) &main_func_index, sizeof (int));

  
  // load instruction stream
  
  // load string table

  // load function table
 
  binary.close ();
}

