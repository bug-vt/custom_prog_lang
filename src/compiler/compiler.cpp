#include "lexer.hpp"
#include "preprocess.hpp"
#include "parser.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include "ast_printer.hpp"
#include "emitter.hpp"
#include "cleaner.hpp"

using std::ifstream;
using std::ofstream;
using std::stringstream;
using std::cout;
using std::endl;
using std::string;
using std::vector;

void exitOnError (string msg);
string loadSourceFile (char *file_name);

int main (int argc, char **argv)
{
  if (argc < 2)
    exitOnError ("usage: compiler source_file <output_file_name>");
 
  string out_file_name = "out.casm";
  if (argc > 2)
    out_file_name = string (argv[2]) + ".casm";

  string raw_source = loadSourceFile (argv[1]);
  Parser parser (raw_source);
  vector<Stmt*> statements = parser.parse ();

  AstPrinter printer;
  std::cout << printer.print (statements);

  ofstream out_file (out_file_name);
  Emitter emitter;
  out_file << emitter.walkAst (statements);

  Cleaner cleaner;
  cleaner.cleanAst (statements);

  out_file.close ();
  return 0;
}

string loadSourceFile (char *file_name)
{
  // open the input file
  ifstream input_file (file_name);
  
  if (input_file.fail ())
    exitOnError ("fail to open given input file: " + string (file_name));

  // load source code into memory
  stringstream buffer;
  buffer << input_file.rdbuf ();

  // close the file
  input_file.close ();

  return buffer.str ();
}

void exitOnError (string msg)
{
  cout << "Error: " << msg << endl;
  exit (EXIT_FAILURE);
}
