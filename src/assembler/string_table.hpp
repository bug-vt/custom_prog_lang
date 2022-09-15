#ifndef _STRING_TABLE
#define _STRING_TABLE

#include <vector>
#include <string>

class StringTable
{
  public:
    StringTable ();
    int addString (std::string str);
    std::string at (int index);
    int size ();

  private:
    int str_count;
    std::vector<std::string> str_table;
};

#endif
