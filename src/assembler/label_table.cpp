#include "label_table.hpp"
#include <iostream>

using std::string;
using std::unordered_map;

Label::Label (string ident, int func_index)
{
  this->ident = ident;
  this->func_index = func_index;
}

LabelInfo::LabelInfo (int index, int target) : label_index (index),
                                               target_index (target)
{
}

LabelTable::LabelTable () : label_count (0), label_table ()
{
}

// Add label to the label table.
// If the label is already exists inside the table, return -1.
// Otherwise, return the assigned index that would use to locate 
// the label inside the table.
int LabelTable::addLabel (Label label, int target_index)
{
  // check if given function is already inside the table.
  if (label_table.find (label) != label_table.end ())
    return -1;

  // add the given function into the table.
  int curr_count = label_count;
  LabelInfo curr_label_info (label_count, target_index);
  label_table[label] = curr_label_info;
  label_count++;

  return curr_count;
  
}

LabelInfo LabelTable::getLabel (Label label)
{
  return label_table.at (label);
}

int LabelTable::getTargetIndex (Label label)
{
  int target_index;
  try
  {
    target_index = getLabel (label).target_index;
  }
  catch (...)
  {
    target_index = -1;
  }
  return target_index;
}

void LabelTable::print ()
{
  std::cout << "---------------------------------------" << std::endl;
  for (auto const& x : label_table)
  {
    std::cout << x.first.ident << " " << x.first.func_index << "\t";
    std::cout << x.second.target_index << std::endl;
  }
  std::cout << "---------------------------------------" << std::endl;
}
