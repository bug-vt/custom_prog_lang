#ifndef _STRING_TABLE
#define _STRING_TABLE

#include <unordered_map>
#include <string>

class StringTable
{
  public:
    StringTable ();
    int addString (std::string str);

  private:
    int str_index;
    std::unordered_map<std::string, int> str_table;
};

#endif
