#include "string_table.hpp"

using std::vector;
using std::string;

StringTable::StringTable () : str_index (0), str_table ()
{
}

// Add STR to the string table.
// return the assigned key that would use to locate STR inside the table.
int StringTable::addString (string str)
{
  for (int i = 0; i < str_table.size (); i++)
  {
    if (str_table[i] == str)
      return i;
  }

  int curr_index = str_index;
  str_table.push_back (str);
  str_index++;

  return curr_index;
}

string StringTable::at (int index)
{
  return str_table[index];
}

int StringTable::size ()
{
  return str_table.size ();
}

