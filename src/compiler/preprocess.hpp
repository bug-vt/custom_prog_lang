#ifndef PREPROCESS_HPP
#define PREPROCESS_HPP

#include <vector>
#include <string>

struct Preprocess
{
  static void removeComments (std::vector<std::string> &source_code);
};

#endif
