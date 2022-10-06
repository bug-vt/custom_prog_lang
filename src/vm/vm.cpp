#include "script.hpp"
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;


int main (int argc, char** argv)
{
  if (argc < 2)
  {
    cout << "usage: vm executable_file" << endl;
    return 1;
  }

  string input_file = string (argv[1]);
  Script script;
  script.load (input_file);
  script.reset (argc, argv);
  script.execute ();
}
