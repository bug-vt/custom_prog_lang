#include "label_table.hpp"

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

LabelTable::LabelTable () : label_index (0), label_table ()
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
  int curr_index = label_index;
  LabelInfo curr_label_info (label_index, target_index);
  label_table[label] = curr_label_info;
  label_index++;

  return curr_index;
  
}

LabelInfo LabelTable::getLabel (Label label)
{
  return label_table.at (label);
}
