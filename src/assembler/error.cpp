#include "error.hpp"
#include <iostream>
#include <cstdlib>
#include <algorithm>

using std::cout;
using std::endl;
using std::string;
using std::replace;

void exitOnError (string msg)
{
  cout << "Error: " << msg << endl;
  exit (EXIT_FAILURE);
}

void exitOnCodeError (string msg, AsmLexer &lexer)
{
  cout << "Error: " << msg << endl;
  cout << "Line: " << lexer.getCurrLineIndex () << endl;

  // replace all tabs to white space
  string line = lexer.getCurrLine ();
  replace (line.begin (), line.end (), '\t', ' '); 

  // show error line and caret at the start of the error lexeme
  cout << line;
  cout << string (lexer.getLexemeStartIndex (), ' ') << '^' << endl;

  cout << "Fail to assemble\n" << endl;
  exit (EXIT_FAILURE);
}
