/*
 * This implement virtual machine for running custom assembly.
 * The current VM implement single cycle execution and it can run one
 * executable at a time.
 * The general structure of VM was adapted from the Alex Varanese's XVM.
 */


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
    cout << "usage: vm executable_file [argument]..." << endl;
    return 1;
  }

  string input_file = string (argv[1]);
  Script script;
  script.load (input_file);
  script.reset (argc, argv);
  script.execute ();
}
