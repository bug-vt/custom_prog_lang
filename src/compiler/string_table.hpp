#ifndef STRING_TABLE_HPP
#define STRING_TABLE_HPP

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
