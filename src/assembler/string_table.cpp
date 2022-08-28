#include "string_table.hpp"

using std::unordered_map;
using std::string;

StringTable::StringTable () : key (0), str_table ()
{
}

// Add STR to the string table.
// return the assigned key that would use to locate STR inside the table.
int StringTable::addString (string str)
{
  if (str_table.find (str) != str_table.end ())
    return str_table[str];

  int curr_index = str_index;
  str_table[str] = str_index;
  str_index++;

  return curr_index;
}

